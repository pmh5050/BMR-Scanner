#include <fstream>

#include "AScanner.h"
#include "AScanCamera.h"
#include "ACheckerBoard.h"
#include "ALineLaser.h"
#include "AScannerHelper.h"
#include "ALinkedList.h"
#include "AScanDataSet.h"
#include "AUserInterface.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#define ASCANNER_DEBUG false

#define CAMERA_PARAMETER_FILE_PATH "PGCalibration.txt"
#define DETECTOR_PARAMETER_FILE_PATH "DetectorParameter.yml"

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define RANGE_MINIMUM 0
#define RANGE_MAXIMUM 1

AScanner::AScanner(int argc, char **argv)
{
	Application = new QApplication(argc, argv);
	UserInterface = new AUserInterface();
	ScanCamera = new AScanCamera(ECameraType::PointGrey);
	HCheckerBoard = new ACheckerBoard(7, 7, 0.030f, 0.015f, 5);
	VCheckerBoard = new ACheckerBoard(7, 7, 0.030f, 0.015f, 10);
	LineLaser = new ALineLaser();
	LinkedListHead = new ALinkedList();

	ScanCamera->ReadCameraParameters(CAMERA_PARAMETER_FILE_PATH);
	HCheckerBoard->ReadDetectorParameters(DETECTOR_PARAMETER_FILE_PATH);
	VCheckerBoard->ReadDetectorParameters(DETECTOR_PARAMETER_FILE_PATH);

	UserInterface->show();
	UserInterface->SetScannerReference(this);

	bIsValidPose = false; // 매 루프마다 초기화됩니다.
	bIsHLineReady = false;
	bIsVLineReady = false;
	bIsScanDataReady = false;

	UpdateCenterPoint();
}

/**
* 검출의 대상이 되는 CheckerBoard를 설정합니다.
* @param NewObjectCheckerBoardType - 'H'일 경우 H Checkerboard가 검출의 대상이 되며, 'V'일 경우 V Checkerboard가 검출의 대상이 됩니다.
*/
void AScanner::SetObjectCheckerBoard(char NewObjectCheckerBoardType)
{
	switch (NewObjectCheckerBoardType)
	{
	case 'H':
		ObjectCheckerBoard = HCheckerBoard;
		break;
	case 'V':
		ObjectCheckerBoard = VCheckerBoard;
		break;
	default:
		cout << "ERROR : 해당 CheckerBoard Type의 경우 존재하지 않습니다." << endl;
		break;
	}
}

/** 검출의 대상이 되는 ChekcerBoard의 Pointer를 반환합니다. */
class ACheckerBoard* AScanner::GetObjectCheckerBoard()
{
	return ObjectCheckerBoard;
}

/** ObjectCheckerBoard를 검출한 뒤, member class에 해당 정보를 저장합니다. */
void AScanner::DetectCheckerBoard(Mat& Frame)
{
	bIsValidPose = false;

	ScanCamera->GetFrame(Frame);
	aruco::detectMarkers(Frame, ObjectCheckerBoard->Dictionary, MarkerCorners, MarkerIds, ObjectCheckerBoard->DetectorParams, RejectedMarkers);

	// ArUco marker가 1개 이상 검출되었을 경우
	if (MarkerIds.size() > 0)
	{
		aruco::interpolateCornersCharuco(MarkerCorners, MarkerIds, Frame, ObjectCheckerBoard->Charucoboard, CharucoCorners, CharucoIds, ScanCamera->mCamMatrix, ScanCamera->mDistCoeffs);

		// Camera matrix의 차원을 검사하여 크기가 0이 아닌지 확인합니다
		if (ScanCamera->mCamMatrix.total() != 0)
		{
			// ArUco 함수를 호출하여 유효한 추정인지 여부를 '자세 추정 성공 여부 변수'로 저장합니다.
			// 여부를 확인받는 것과 동시에 Rotation matrix와 Translate matrix의 주소를 인자로 넣어서 값을 저장합니다.

			bIsValidPose = aruco::estimatePoseCharucoBoard(CharucoCorners, CharucoIds, ObjectCheckerBoard->Charucoboard, ScanCamera->mCamMatrix, ScanCamera->mDistCoeffs, vRot, vTrs);

			/*
			Mat Ids(CharucoIds);

			if (Ids.total() > 3)
			{

				vector< Point3f > ObjPoints;
				ObjPoints.reserve(Ids.total());
				for (unsigned int i = 0; i < Ids.total(); i++) {
					int currId = Ids.at<int>(i);
					// CV_Assert(currId >= 0 && currId < (int)_board->chessboardCorners.size());
					ObjPoints.push_back(ObjectCheckerBoard->Charucoboard->chessboardCorners[currId]);
				}
				solvePnP(ObjPoints, CharucoCorners, ScanCamera->mCamMatrix, ScanCamera->mDistCoeffs, vRot, vTrs);

				// (수정) : bIsValidPose의 판단이 엄격하여, Marker의 개수를 통해 추정 유무를 판단하는 것으로 변경합니다.
				bIsValidPose = true;
			}
			else
			{
				bIsValidPose = false;
			}
			*/
		}
	}
}

/** ObjectCheckerBoard의 Rotation matrix 및 Translate matrix를 이용하여 Transform maxtrix를 Update 합니다. */
void AScanner::UpdateCheckerBoardTransformMatrix()
{
	if (bIsValidPose)
	{
		double dTransformMatrix[4][4] = { 0 };
		dTransformMatrix[3][3] = 1.0f;

		Mat R;

		Rodrigues(vRot, R);
		for (int i = 0; i < 3; i++)
		{
			double* MatrixPointer = R.ptr<double>(i);
			for (int j = 0; j < 3; j++)
			{
				dTransformMatrix[i][j] = MatrixPointer[j];
			}
		}

		for (int i = 0; i < 3; i++)
		{
			dTransformMatrix[i][3] = vTrs[i];
		}

		Mat TransformMatrix(4, 4, CV_64F, dTransformMatrix);
		ObjectCheckerBoard->SetTransformB2C(TransformMatrix);
	}
}

/** Frame을 입력받아 검출한 CheckerBoard의 축을 표시합니다. */
void AScanner::DrawCheckerBoardAxis(Mat& Frame)
{
	// '자세 추정 성공 여부 변수'의 값이 참일 경우
	if (bIsValidPose)
	{
		// 축을 그립니다 
		aruco::drawAxis(Frame, ScanCamera->mCamMatrix, ScanCamera->mDistCoeffs, vRot, vTrs, ObjectCheckerBoard->AxisLength);
	}
}

/** Frame을 입력받아 검출한 CheckerBoard의 축을 Offset Option을 고려하여 표시합니다. */
void AScanner::DrawCheckerBoardAxis(Mat& Frame, EOffsetType OffsetType)
{
	if (bIsValidPose)
	{
		switch (OffsetType)
		{
		case EOffsetType::CenterPoint:
		{
			UpdateCheckerBoardTransformMatrix();
			Mat TransformB2C = ObjectCheckerBoard->GetTransformB2C();
			/*
			static double BaseYaw = AScannerHelper::CalYaw(AScannerHelper::GetRotationMatrix(TransformB2C));
			double CurrentYaw = AScannerHelper::CalYaw(AScannerHelper::GetRotationMatrix(TransformB2C));
			double DeltaYaw = (CurrentYaw - BaseYaw) * 180.0f / CV_PI;
			cout << DeltaYaw << endl;
			StandardDevReport << DeltaYaw << endl;
			*/
			Mat DeltaOffset = (TransformB2C * CenterPoint);

			double* OffsetPointer = DeltaOffset.ptr<double>(0);
			for (int i = 0; i < 3; i++)
			{
				vTrs[i] = OffsetPointer[i];
			}

			break;
		}

		case EOffsetType::None:
		{
			break;
		}
		default:
			// ERROR
			break;
		}

		// 축을 그립니다 
		aruco::drawAxis(Frame, ScanCamera->mCamMatrix, ScanCamera->mDistCoeffs, vRot, vTrs, ObjectCheckerBoard->AxisLength);
	}
}

/** CheckerBoard의 Normal Vector와 Pivot Point를 Camera Coordinate에 대해서 Update 합니다. */
void AScanner::UpdateCheckerBoardParams()
{
	if (bIsValidPose)
	{
		double dNormalVector[] = { 0, 0, 1, 0 };
		Mat bNormalVector(4, 1, CV_64F, dNormalVector);
		Mat cNormalVector = ObjectCheckerBoard->GetTransformB2C() * bNormalVector;
		ObjectCheckerBoard->SetNormalVector(cNormalVector);

		double dPivotPoint[] = { 0, 0, 0, 1 };
		Mat bPivotPoint(4, 1, CV_64F, dPivotPoint);
		Mat cPivotPoint = ObjectCheckerBoard->GetTransformB2C() * bPivotPoint;
		ObjectCheckerBoard->SetPivotPoint(cPivotPoint);

		double dPlaneParams[4] = {};
		double* NormalVectorPointer = cNormalVector.ptr<double>(0);
		double* PivotPointPointer = cPivotPoint.ptr<double>(0);
		for (int i = 0; i < 3; i++)
		{
			dPlaneParams[i] = NormalVectorPointer[i]; // a, b, c
			dPlaneParams[3] -= NormalVectorPointer[i] * PivotPointPointer[i]; // d
		}
		Mat PlaneParams(1, 4, CV_64F, dPlaneParams);
		ObjectCheckerBoard->SetPlaneParams(PlaneParams);
	}
}

/** 선택된 CheckerBoard의 종류에 따라 Line Update를 수행합니다 */
void AScanner::UpdateCheckerBoardLaserLine(Mat& Frame)
{
	if (bIsValidPose)
	{
		int* RedPointPosition = new int[HEIGHT_SIZE];
		AScannerHelper::CalcRedPointUseGaussianBlur(Frame, LineLaser->GetMinimumRedValueThreshold(), RedPointPosition);

		Mat LineVector = AScannerHelper::CalLinearRegression(ObjectCheckerBoard, ScanCamera, RedPointPosition);
		if (AScannerHelper::IsRegressionCompleted())
		{
			ObjectCheckerBoard->SetLaserLineVector(LineVector);
			if (ObjectCheckerBoard == HCheckerBoard) // 수평면일경우 Pivot Point를 추가로 계산합니다
			{
				Mat PivotPoint = AScannerHelper::CalLinePivotPoint(LineVector, 0.0f);
#if ASCANNER_DEBUG
				cout << "Line Params : " << LineVector << endl;
				cout << "Pivot Point : " << PivotPoint << endl;
#endif
				ObjectCheckerBoard->SetLaserPivotPoint(PivotPoint);

				UserInterface->SetHLine(LineVector);
				bIsHLineReady = true;
			}
			else if (ObjectCheckerBoard == VCheckerBoard)
			{
				UserInterface->SetVLine(LineVector);
				bIsVLineReady = true;
			}
		}

		delete RedPointPosition;
	}
}

/** 현재 Frame이 Pose가 존재하는지 여부를 반환합니다 */
bool AScanner::IsValidPose()
{
	return bIsValidPose;
}

/** WindowHandler를 이용하여 CurrentCursorData를 Update 합니다 */
void AScanner::UpdateCursorData()
{
	WindowHandler = (HWND)cvGetWindowHandle("Frame");
	GetCursorPos(&CurrentCursorData);
	ScreenToClient(WindowHandler, &CurrentCursorData);
}

/** Checkerboard들의 Line Params를 이용하여, Camera coordinate 기준 Line laser의 Plane Params를 Update 합니다. */
void AScanner::UpdateLineLaserPlaneParams()
{
	if (bIsHLineReady && bIsVLineReady)
	{
		cout << "Update Line Laser Plane Parameters!" << endl;

		// Line Vector format : y = ax + b에서 (a, 1, b)
		double* LineParamsPointer;
		double dHLineVector[3] = { 0 };
		double dVLineVector[3] = { 0 };
		Mat bHLineParams = HCheckerBoard->GetLaserLineVector();
		Mat bVLineParams = VCheckerBoard->GetLaserLineVector();

#if ASCANNER_DEBUG
		cout << "H Params : " << bHLineParams << endl;
		cout << "V Params : " << bVLineParams << endl;
#endif

		LineParamsPointer = bHLineParams.ptr<double>(0);
		dHLineVector[0] = LineParamsPointer[1];
		dHLineVector[1] = LineParamsPointer[0];
		Mat bHLineVector(1, 3, CV_64F, dHLineVector);

		LineParamsPointer = bVLineParams.ptr<double>(0);
		dVLineVector[0] = LineParamsPointer[1];
		dVLineVector[1] = LineParamsPointer[0];
		Mat bVLineVector(1, 3, CV_64F, dVLineVector);

#if ASCANNER_DEBUG
		cout << "Converting Line Params to Line Vector is Completed" << endl;
		cout << "H Vector : " << bHLineVector << endl;
		cout << "V Vector : " << bVLineVector << endl;
#endif

		Mat HTransformMatrix = HCheckerBoard->GetTransformB2C();
		Mat VTransformMatrix = VCheckerBoard->GetTransformB2C();

		Mat bPivotPoint = HCheckerBoard->GetLaserPivotPoint();
#if ASCANNER_DEBUG
		cout << "Pivot Point : " << bPivotPoint << endl;
#endif

		Mat cHLineVector = AScannerHelper::LineVectorCoordinateTransform(bHLineVector, HTransformMatrix);
		Mat cVLineVector = AScannerHelper::LineVectorCoordinateTransform(bVLineVector, VTransformMatrix);

		Mat cNormalVector = AScannerHelper::CalCrossProduct(cHLineVector, cVLineVector);
#if ASCANNER_DEBUG
		cout << "NormalVector in Camera Coordinate : " << cNormalVector << endl;
#endif

		Mat cPivotPoint = AScannerHelper::PointCoordinateTransform(bPivotPoint, HTransformMatrix);

#if ASCANNER_DEBUG
		cout << "Ready for Plane params" << endl;
		cout << "Normal Vector(Camera coordinate) : " << cNormalVector << endl;
		cout << "Pivot Point(Camera coordiante) : " << cPivotPoint << endl;
#endif

		double* NormalVectorPointer = cNormalVector.ptr<double>(0);
		double* PivotPointPointer = cPivotPoint.ptr<double>(0);

		double dLaserLinePlaneParams[4];

		dLaserLinePlaneParams[3] = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			dLaserLinePlaneParams[i] = NormalVectorPointer[i];
			dLaserLinePlaneParams[3] -= NormalVectorPointer[i] * PivotPointPointer[i];// -(ax + by + cz) = d
		}

		Mat LaserLinePlaneParams(1, 4, CV_64F, dLaserLinePlaneParams);

#if ASCANNER_DEBUG
		cout << "Laser Line Plane Params for Camera Coordinate" << endl;
		cout << LaserLinePlaneParams << endl;
#endif

		LineLaser->SetPlaneParams(LaserLinePlaneParams);
		UserInterface->SetLaserPlane(LaserLinePlaneParams);
	}
	else
	{
		if (!bIsHLineReady)
		{
			cout << "H Line 정보가 필요합니다." << endl;
		}
		if (!bIsVLineReady)
		{
			cout << "V Line 정보가 필요합니다." << endl;
		}
	}

	UserInterface->UserInterfaceAccessor.UpdateNoneBox->setChecked(true);
}

/** Scan data가 존재하는지 여부를 반환합니다. */
bool AScanner::IsScanDataReady()
{
	return bIsScanDataReady;
}

/** Scan을 1회 진행합니다. (타이머를 수행하지 않습니다.) */
void AScanner::ScanRunningUntilTimer()
{
	Mat CurrentFrame;
	ScanCamera->GetFrame(CurrentFrame);
	SetObjectCheckerBoard('H');
	DetectCheckerBoard(CurrentFrame);

}

/** Scan을 진행합니다. */
void AScanner::ScanRunning()
{
	Mat CurrentFrame;
	const int TimeDelay = 1000;
	const int SettlingTime = 800;
	ScanCamera->GetFrame(CurrentFrame);
	SetObjectCheckerBoard('H');
	DetectCheckerBoard(CurrentFrame);

	if (!bIsScanDataReady && bIsValidPose)
	{
		cout << "Scan" << endl;
		UserInterface->TimerInit(TimeDelay);
		UserInterface->WorldTimeManager.start();

	}

	// 최초 1회 Measurment 검출 이후에는 Motion Model로도 Scan을 진행하게 끔 합니다.
	int DeltaTime = UserInterface->GetDeltaTime();
	if (bIsValidPose || (DeltaTime > SettlingTime))
	{
		// 포즈가 검출될 경우만 수행됩니다.
		UpdateCheckerBoardTransformMatrix();

		int StepCount = UserInterface->GetStepCount();

		static int PrevStepCount = 0;
		static int CurrentStepCount;
		CurrentStepCount = StepCount;

		int DeltaStepCount = CurrentStepCount - PrevStepCount;
		PrevStepCount = CurrentStepCount;

		if (DeltaStepCount != 0 || bIsValidPose)
		{
			ALinkedList* LinkedListPointer;
			if (!bIsScanDataReady)
			{
				bIsScanDataReady = true;
				LinkedListPointer = LinkedListHead;
			}
			else
			{
				LinkedListPointer = new ALinkedList();
				LinkedListHead->InsertNextNode(LinkedListPointer);
			}

			AScanDataSet* ScanDataSet = LinkedListPointer->GetDataSetPtr();
			Mat TransformB2C = ObjectCheckerBoard->GetTransformB2C();

			int MarkerCount = MarkerIds.size();

			cout << "DeltaTime : " << DeltaTime << endl;
			cout << "MarkerCount : " << MarkerCount << endl;
			cout << "StepCount : " << StepCount << endl;
			cout << "IsValidPose : " << bIsValidPose << endl;
			ScanDataSet->SetOrgImageData(CurrentFrame);
			ScanDataSet->SetTransformB2C(TransformB2C);
			ScanDataSet->SetIsValidPose(bIsValidPose);
			ScanDataSet->SetDeltaTimeMs(DeltaTime);
			ScanDataSet->SetDetectedMarkerCount(MarkerCount);
			ScanDataSet->SetDeltaStepCount(DeltaStepCount);

			// Additional option ( for task test )
			ScanDataSet->SetElapsedTime(UserInterface->WorldTimeManager.elapsed());
		}
	}
	else
	{
#if ASCANNER_DEBUG
		cout << "Fail for Board detect" << endl;
#endif
	}
}

/** Scan data로부터 Point Cloud 정보를 계산한 뒤 해당 FileName에 저장합니다 */
void AScanner::StorePointCloud(string FileName)
{
	ofstream WriteFile(FileName.data());
	if (!WriteFile.is_open())
	{
#if ASCANNER_DEBUG
		cout << "Data 기록을 위한 파일을 open할 수 없습니다." << endl;
#endif
	}
	if (bIsScanDataReady)
	{
#if ASCANNER_DEBUG
		cout << "Line Laser plane : " << LineLaser->GetPlaneParams() << endl;
#endif
		// 연산
		ALinkedList* LinkedListPointer = LinkedListHead;
		int* FramePoint = new int[HEIGHT_SIZE]; // 할당
		double* dRayVector = new double[3]; // 할당
		double* IntersectionPointPointer;
		double BoardPosition[3]; // x, y, z
		do
		{
			AScanDataSet* NodeScanDataSet = LinkedListPointer->GetDataSetPtr();
			Mat NodeFrame = NodeScanDataSet->GetOrgImageData();
			Mat GrayFrame = AScannerHelper::GetLineLaserFrame(NodeFrame, LineLaser->GetMinimumRedValueThreshold());


			imshow("Scan Node Frame", NodeFrame);
			imshow("Scan Laser Frame", GrayFrame);
			waitKey(10);

			AScannerHelper::CalcRedPoint(NodeFrame, LineLaser->GetMinimumRedValueThreshold(), FramePoint);

			for (int i = 0; i < HEIGHT_SIZE; i++)
			{
				if (FramePoint[i] != -1)
				{
					AScannerHelper::CalRayVector(FramePoint[i], i, ScanCamera, dRayVector);
					Mat TransformC2B = NodeScanDataSet->GetTransformB2C().inv();
					Mat RayVector(1, 3, CV_64F, dRayVector);
					Mat cIntersectionPoint = AScannerHelper::CalIntersectionPoint(LineLaser->GetPlaneParams(), RayVector);
					Mat bIntersectionPoint = AScannerHelper::PointCoordinateTransform(cIntersectionPoint, TransformC2B);

#if ASCANNER_DEBUG
					cout << "cIntersection point [" << i << "] : " << cIntersectionPoint << endl;
					cout << "bIntersection point [" << i << "] : " << bIntersectionPoint << endl;
#endif
					IntersectionPointPointer = bIntersectionPoint.ptr<double>(0);
					for (int j = 0; j < 3; j++)
					{
						BoardPosition[j] = IntersectionPointPointer[j];
					}
					WriteFile << BoardPosition[0] << " " << BoardPosition[1] << " " << BoardPosition[2] << endl;
				}
			}
			cout << "Before : " << LinkedListPointer << endl;
			LinkedListPointer = LinkedListPointer->GetNextNodePtr();
			cout << "Next : " << LinkedListPointer << endl;
		} while (LinkedListPointer != nullptr);

		WriteFile.close();
		delete FramePoint;
		delete dRayVector;
	}
	else
	{
		cout << "Scan Data가 존재하지 않습니다." << endl;
		WriteFile << "Scan Data doesn't exist" << endl;
	}
}

/**
* Scan data로부터 Point Cloud 정보를 계산한 뒤 해당 FileName에 저장합니다, 추가로 ScanVolume을 입력받아서 해당 범위를 벗어난 경우 Point에서 열외합니다.
* @param Scan Volume - 첫 번째 index는 Axis에 해당하며, 두 번째 index의 경우 0이면 minimum, 1이면 maximum에 해당합니다.
*/
void AScanner::StorePointCloud(string FileName, double ScanVolume[3][2])
{
	ofstream WriteFile(FileName.data());
	if (!WriteFile.is_open())
	{
#if ASCANNER_DEBUG
		cout << "Data 기록을 위한 파일을 open할 수 없습니다." << endl;
#endif
		return;
	}

	if (bIsScanDataReady)
	{
#if ASCANNER_DEBUG
		cout << "Line Laser plane : " << LineLaser->GetPlaneParams() << endl;
#endif
		// 연산
		ALinkedList* LinkedListPointer = LinkedListHead;
		// int* FramePoint = new int[HEIGHT_SIZE]; // 할당
		double* FramePoint = new double[HEIGHT_SIZE]; // 할당
		double* dRayVector = new double[3]; // 할당
		double* IntersectionPointPointer;
		double BoardPosition[3]; // (x, y, z) in CheckerBoard Coordinate

		Mat PivotTransformMatrixB2C = LinkedListHead->GetDataSetPtr()->GetTransformB2C();
		cout << "PivotTransformMatrix : " << PivotTransformMatrixB2C << endl;

		Mat OptimalCenterPoint = AScannerHelper::CalOptimalCenterPointVector(LinkedListHead);
		cout << "회전 중심 좌표 : " << OptimalCenterPoint << endl;

		UpdateOptimalPoint(OptimalCenterPoint);

		cout << "Center Point in (x, y, z) : " << CenterPoint << endl;
		cout << "Optimal Point int (x, y, z) : " << OptimalPoint << endl;

		while (true)
		{
			AScanDataSet* NodeScanDataSet = LinkedListPointer->GetDataSetPtr();

			// 예외처리
			if (NodeScanDataSet->IsReadyImgData() == false)
			{
				int Dummy;
				cout << "Empty Data Detected!" << endl;
				cout << "Please any key : ";
				cin >> Dummy;
				continue;
			}

			Mat NodeFrame = NodeScanDataSet->GetOrgImageData();
			Mat NodeLaserFrame = AScannerHelper::GetLineLaserFrame(NodeFrame, LineLaser->GetMinimumRedValueThreshold());

			imshow("Node Frame", NodeFrame);
			imshow("Node Laser Frame", NodeLaserFrame);
			char Key = waitKey(1);

			// AScannerHelper::CalcRedPointUseGaussianBlur(NodeFrame, LineLaser->GetMinimumRedValueThreshold(), FramePoint);
			AScannerHelper::CalcRedPointUseGaussianBlur(NodeFrame, LineLaser->GetMinimumRedValueThreshold(), FramePoint, 8);
			Mat ObjectTransformMatrixB2C = NodeScanDataSet->GetTransformB2C();
			Mat TransformC2O = AScannerHelper::CalOptimalTransformMatrix(OptimalCenterPoint, ObjectTransformMatrixB2C, PivotTransformMatrixB2C); // Camera Coordinate to Opmial Coordinate

			Mat CameraPosition = AScannerHelper::GetTranslateMatrix(TransformC2O);

			double* CameraPositionPointer = CameraPosition.ptr<double>(0);
			WriteFile << CameraPositionPointer[0] << " " << CameraPositionPointer[1] << " " << CameraPositionPointer[2] << endl;
			for (int i = 0; i < HEIGHT_SIZE; i++)
			{
				if (FramePoint[i] != -1)
				{
					AScannerHelper::CalRayFVector(FramePoint[i], i, ScanCamera, dRayVector); // 실수형 계산
					// Mat TransformC2B = NodeScanDataSet->GetTransformB2C().inv(); // Default Option

					Mat RayVector(1, 3, CV_64F, dRayVector);
					Mat cIntersectionPoint = AScannerHelper::CalIntersectionPoint(LineLaser->GetPlaneParams(), RayVector);
					// Mat bIntersectionPoint = ScannerHelper->PointCoordinateTransform(cIntersectionPoint, TransformC2B); // Default Option
					Mat bIntersectionPoint = AScannerHelper::PointCoordinateTransform(cIntersectionPoint, TransformC2O); // Global Optimization Option

					Mat IntersectionPoint = AScannerHelper::GetOffsetCoordinate(EOffsetType::CenterPoint, bIntersectionPoint, CenterPoint, OptimalPoint);
#if ASCANNER_DEBUG
					cout << "cIntersection point [" << i << "] : " << cIntersectionPoint << endl;
					cout << "bIntersection point [" << i << "] : " << bIntersectionPoint << endl;
#endif
					IntersectionPointPointer = IntersectionPoint.ptr<double>(0);
					for (int j = 0; j < 3; j++)
					{
						BoardPosition[j] = IntersectionPointPointer[j];
					}

					bool bIsVolumeIn = true;
					for (int j = 0; j < 3; j++)
					{
						if (ScanVolume[j][RANGE_MINIMUM] <= BoardPosition[j] && BoardPosition[j] <= ScanVolume[j][RANGE_MAXIMUM])
						{
							// 해당 Axis의 조건을 만족합니다.
						}
						else
						{
							bIsVolumeIn = false;
						}
					}

					if (true)
					{
						// Point가 입력받은 Volume 내에 위치합니다.
						WriteFile << BoardPosition[0] << " " << BoardPosition[1] << " " << BoardPosition[2] << endl;
					}
				}
			}
			if (LinkedListPointer->GetNextNodePtr() != nullptr)
			{
				LinkedListPointer = LinkedListPointer->GetNextNodePtr();
			}
			else
			{
				break;
			}
		}
		WriteFile.close();
		delete[] FramePoint;
		delete[] dRayVector;
	}
	else
	{
		cout << "Scan Data가 존재하지 않습니다." << endl;
	}

}

/**
* Scan data로부터 Point Cloud 정보를 계산한 뒤 해당 FileName에 저장합니다, 추가로 ScanVolume을 입력받아서 해당 범위를 벗어난 경우 Point에서 열외합니다.
* @param Scan Volume - 첫 번째 index는 Axis에 해당하며, 두 번째 index의 경우 0이면 minimum, 1이면 maximum에 해당합니다.
* @param WindowSize - Anti-Aliasing에 사용될 Windows의 크기에 해당합니다.
*/
void AScanner::StorePointCloud(string FileName, double ScanVolume[3][2], int WindowSize)
{
	ofstream WriteFile(FileName.data());
	ofstream FilterFile("FilterData.txt");

	if (!WriteFile.is_open())
	{
#if ASCANNER_DEBUG
		cout << "Data 기록을 위한 파일을 open할 수 없습니다." << endl;
#endif
		return;
	}

	if (!FilterFile.is_open())
	{
#if ASCANNER_DEBUG
		cout << "Data 기록을 위한 파일을 open할 수 없습니다." << endl;
#endif
		return;
	}

	if (bIsScanDataReady)
	{
#if ASCANNER_DEBUG
		cout << "Line Laser plane : " << LineLaser->GetPlaneParams() << endl;
#endif
		// 연산
		ALinkedList* LinkedListPointer = LinkedListHead;
		// int* FramePoint = new int[HEIGHT_SIZE]; // 할당
		double* FramePoint = new double[HEIGHT_SIZE]; // 할당
		double* dRayVector = new double[3]; // 할당
		double* IntersectionPointPointer;
		double BoardPosition[3]; // (x, y, z) in CheckerBoard Coordinate

		Mat PivotTransformMatrixB2C = LinkedListHead->GetDataSetPtr()->GetTransformB2C();
		cout << "PivotTransformMatrix : " << PivotTransformMatrixB2C << endl;

		Mat OptimalCenterPoint = AScannerHelper::CalOptimalCenterPointVectorOfValidPoseNode(LinkedListHead);
		cout << "회전 중심 좌표 : " << OptimalCenterPoint << endl;

		UpdateCenterPoint();
		UpdateOptimalPoint(OptimalCenterPoint);

		cout << "Center Point in (x, y, z) : " << CenterPoint << endl;
		cout << "Optimal Point int (x, y, z) : " << OptimalPoint << endl;

		while (true)
		{
			AScanDataSet* NodeScanDataSet = LinkedListPointer->GetDataSetPtr();

			// 예외처리
			if (NodeScanDataSet->IsReadyImgData() == false)
			{
				int Dummy;
				cout << "Empty Data Detected!" << endl;
				cout << "Please any key : ";
				cin >> Dummy;
				continue;
			}

			Mat NodeFrame = NodeScanDataSet->GetOrgImageData();
			Mat NodeLaserFrame = AScannerHelper::GetLineLaserFrame(NodeFrame, LineLaser->GetMinimumRedValueThreshold());
			
			// UserInterface->UpdateLabelFromFrame(ELabelType::RGBFrame, NodeFrame.clone());
			// UserInterface->UpdateLabelFromFrame(ELabelType::LaserFrame, NodeLaserFrame.clone());
			imshow("Node Frame", NodeFrame);
			imshow("Node Laser Frame", NodeLaserFrame);
			char Key = waitKey(10);

			// AScannerHelper::CalcRedPointUseGaussianBlur(NodeFrame, LineLaser->GetMinimumRedValueThreshold(), FramePoint);
			AScannerHelper::CalcRedPointUseGaussianBlur(NodeFrame, LineLaser->GetMinimumRedValueThreshold(), FramePoint, WindowSize);
			Mat ObjectTransformMatrixB2C = NodeScanDataSet->GetTransformB2C();
			
			Mat TransformC2O = AScannerHelper::CalOptimalTransformMatrixUseKF(OptimalCenterPoint, NodeScanDataSet, UserInterface->OdometryCC, UserInterface->MeasurementCC, PivotTransformMatrixB2C); // Camera Coordinate to Opmial Coordinate
			
			/* No KF data 저장용
			if (NodeScanDataSet->GetIsValidPose())
			{
				FilterFile << NodeScanDataSet->GetElpasedTime() << " " << NodeScanDataSet->GetDeltaAngle() << endl;

			}
			else
			{
				FilterFile << NodeScanDataSet->GetElpasedTime() << " " << 0.0f << endl;

				if (LinkedListPointer->GetNextNodePtr() != nullptr)
				{
					LinkedListPointer = LinkedListPointer->GetNextNodePtr();
					continue;
				}
				else
				{
					break;
				}
			}
			*/
			FilterFile << NodeScanDataSet->GetElpasedTime() << " " << NodeScanDataSet->GetDeltaAngle() << endl;

			// Mat CameraPosition = AScannerHelper::GetTranslateMatrix(TransformC2O);
			// double* CameraPositionPointer = CameraPosition.ptr<double>(0);
			// WriteFile << CameraPositionPointer[0] << " " << CameraPositionPointer[1] << " " << CameraPositionPointer[2] << endl;
			for (int i = 0; i < HEIGHT_SIZE; i++)
			{
				if (FramePoint[i] != -1)
				{
					AScannerHelper::CalRayFVector(FramePoint[i], i, ScanCamera, dRayVector); // 실수형 계산
					// Mat TransformC2B = NodeScanDataSet->GetTransformB2C().inv(); // Default Option

					Mat RayVector(1, 3, CV_64F, dRayVector);
					Mat cIntersectionPoint = AScannerHelper::CalIntersectionPoint(LineLaser->GetPlaneParams(), RayVector);
					// Mat bIntersectionPoint = ScannerHelper->PointCoordinateTransform(cIntersectionPoint, TransformC2B); // Default Option
					Mat bIntersectionPoint = AScannerHelper::PointCoordinateTransform(cIntersectionPoint, TransformC2O); // Global Optimization Option

					Mat IntersectionPoint = AScannerHelper::GetOffsetCoordinate(EOffsetType::CenterPoint, bIntersectionPoint, CenterPoint, OptimalPoint);
#if ASCANNER_DEBUG
					cout << "cIntersection point [" << i << "] : " << cIntersectionPoint << endl;
					cout << "bIntersection point [" << i << "] : " << bIntersectionPoint << endl;
#endif
					IntersectionPointPointer = IntersectionPoint.ptr<double>(0);
					for (int j = 0; j < 3; j++)
					{
						BoardPosition[j] = IntersectionPointPointer[j];
					}

					bool bIsVolumeIn = true;
					for (int j = 0; j < 3; j++)
					{
						if (ScanVolume[j][RANGE_MINIMUM] <= BoardPosition[j] && BoardPosition[j] <= ScanVolume[j][RANGE_MAXIMUM])
						{
							// 해당 Axis의 조건을 만족합니다.
						}
						else
						{
							bIsVolumeIn = false;
						}
					}

					if (bIsVolumeIn)
					{
						// Point가 입력받은 Volume 내에 위치합니다.
						WriteFile << BoardPosition[0] << " " << BoardPosition[1] << " " << BoardPosition[2] << endl;
					}
				}
			}
			if (LinkedListPointer->GetNextNodePtr() != nullptr)
			{
				LinkedListPointer = LinkedListPointer->GetNextNodePtr();
			}
			else
			{
				break;
			}
		}
		WriteFile.close();
		FilterFile.close();
		delete[] FramePoint;
		delete[] dRayVector;
	}
	else
	{
		cout << "Scan Data가 존재하지 않습니다." << endl;
	}
}

/**
* Scan data로부터 Point Cloud 정보를 계산한 뒤 해당 FileName에 저장합니다, 추가로 ScanVolume을 입력받아서 해당 범위를 벗어난 경우 Point에서 열외합니다.
* @param Scan Volume - 첫 번째 index는 Axis에 해당하며, 두 번째 index의 경우 0이면 minimum, 1이면 maximum에 해당합니다.
* @param WindowSize - Anti-Aliasing에 사용될 Windows의 크기에 해당합니다.
*/
void AScanner::StorePointCloudWithoutKF(string FileName, double ScanVolume[3][2], int WindowSize)
{
	ofstream WriteFile(FileName.data());
	ofstream FilterFile("NoFilterData.txt");

	if (!WriteFile.is_open())
	{
#if ASCANNER_DEBUG
		cout << "Data 기록을 위한 파일을 open할 수 없습니다." << endl;
#endif
		return;
	}

	if (!FilterFile.is_open())
	{
#if ASCANNER_DEBUG
		cout << "Data 기록을 위한 파일을 open할 수 없습니다." << endl;
#endif
		return;
	}

	if (bIsScanDataReady)
	{
#if ASCANNER_DEBUG
		cout << "Line Laser plane : " << LineLaser->GetPlaneParams() << endl;
#endif
		// 연산
		ALinkedList* LinkedListPointer = LinkedListHead;
		// int* FramePoint = new int[HEIGHT_SIZE]; // 할당
		double* FramePoint = new double[HEIGHT_SIZE]; // 할당
		double* dRayVector = new double[3]; // 할당
		double* IntersectionPointPointer;
		double BoardPosition[3]; // (x, y, z) in CheckerBoard Coordinate

		Mat PivotTransformMatrixB2C = LinkedListHead->GetDataSetPtr()->GetTransformB2C();
		cout << "PivotTransformMatrix : " << PivotTransformMatrixB2C << endl;

		Mat OptimalCenterPoint = AScannerHelper::CalOptimalCenterPointVectorOfValidPoseNode(LinkedListHead);
		cout << "회전 중심 좌표 : " << OptimalCenterPoint << endl;

		UpdateCenterPoint();
		UpdateOptimalPoint(OptimalCenterPoint);

		cout << "Center Point in (x, y, z) : " << CenterPoint << endl;
		cout << "Optimal Point int (x, y, z) : " << OptimalPoint << endl;

		while (true)
		{
			AScanDataSet* NodeScanDataSet = LinkedListPointer->GetDataSetPtr();

			// 예외처리
			if (NodeScanDataSet->IsReadyImgData() == false)
			{
				int Dummy;
				cout << "Empty Data Detected!" << endl;
				cout << "Please any key : ";
				cin >> Dummy;
				continue;
			}

			Mat NodeFrame = NodeScanDataSet->GetOrgImageData();
			Mat NodeLaserFrame = AScannerHelper::GetLineLaserFrame(NodeFrame, LineLaser->GetMinimumRedValueThreshold());

			// UserInterface->UpdateLabelFromFrame(ELabelType::RGBFrame, NodeFrame.clone());
			// UserInterface->UpdateLabelFromFrame(ELabelType::LaserFrame, NodeLaserFrame.clone());
			imshow("Node Frame", NodeFrame);
			imshow("Node Laser Frame", NodeLaserFrame);
			char Key = waitKey(10);

			// AScannerHelper::CalcRedPointUseGaussianBlur(NodeFrame, LineLaser->GetMinimumRedValueThreshold(), FramePoint);
			AScannerHelper::CalcRedPointUseGaussianBlur(NodeFrame, LineLaser->GetMinimumRedValueThreshold(), FramePoint, WindowSize);
			Mat ObjectTransformMatrixB2C = NodeScanDataSet->GetTransformB2C();

			Mat TransformC2O = AScannerHelper::CalOptimalTransformMatrix(OptimalCenterPoint, ObjectTransformMatrixB2C, PivotTransformMatrixB2C);
			// Mat TransformC2O = AScannerHelper::CalOptimalTransformMatrixUseKF(OptimalCenterPoint, NodeScanDataSet, UserInterface->OdometryCC, UserInterface->MeasurementCC, PivotTransformMatrixB2C); // Camera Coordinate to Opmial Coordinate

			// No KF data 저장용
			if (NodeScanDataSet->GetIsValidPose())
			{
				FilterFile << NodeScanDataSet->GetElpasedTime() << " " << NodeScanDataSet->GetDeltaAngle() << endl;
			}
			else
			{
				FilterFile << NodeScanDataSet->GetElpasedTime() << " " << 0.0f << endl;

				if (LinkedListPointer->GetNextNodePtr() != nullptr)
				{
					LinkedListPointer = LinkedListPointer->GetNextNodePtr();
					continue;
				}
				else
				{
					break;
				}
			}
			// FilterFile << NodeScanDataSet->GetElpasedTime() << " " << NodeScanDataSet->GetDeltaAngle() << endl;

			// Mat CameraPosition = AScannerHelper::GetTranslateMatrix(TransformC2O);
			// double* CameraPositionPointer = CameraPosition.ptr<double>(0);
			// WriteFile << CameraPositionPointer[0] << " " << CameraPositionPointer[1] << " " << CameraPositionPointer[2] << endl;
			for (int i = 0; i < HEIGHT_SIZE; i++)
			{
				if (FramePoint[i] != -1)
				{
					AScannerHelper::CalRayFVector(FramePoint[i], i, ScanCamera, dRayVector); // 실수형 계산
					// Mat TransformC2B = NodeScanDataSet->GetTransformB2C().inv(); // Default Option

					Mat RayVector(1, 3, CV_64F, dRayVector);
					Mat cIntersectionPoint = AScannerHelper::CalIntersectionPoint(LineLaser->GetPlaneParams(), RayVector);
					// Mat bIntersectionPoint = ScannerHelper->PointCoordinateTransform(cIntersectionPoint, TransformC2B); // Default Option
					Mat bIntersectionPoint = AScannerHelper::PointCoordinateTransform(cIntersectionPoint, TransformC2O); // Global Optimization Option

					Mat IntersectionPoint = AScannerHelper::GetOffsetCoordinate(EOffsetType::CenterPoint, bIntersectionPoint, CenterPoint, OptimalPoint);
#if ASCANNER_DEBUG
					cout << "cIntersection point [" << i << "] : " << cIntersectionPoint << endl;
					cout << "bIntersection point [" << i << "] : " << bIntersectionPoint << endl;
#endif
					IntersectionPointPointer = IntersectionPoint.ptr<double>(0);
					for (int j = 0; j < 3; j++)
					{
						BoardPosition[j] = IntersectionPointPointer[j];
					}

					bool bIsVolumeIn = true;
					for (int j = 0; j < 3; j++)
					{
						if (ScanVolume[j][RANGE_MINIMUM] <= BoardPosition[j] && BoardPosition[j] <= ScanVolume[j][RANGE_MAXIMUM])
						{
							// 해당 Axis의 조건을 만족합니다.
						}
						else
						{
							bIsVolumeIn = false;
						}
					}

					if (bIsVolumeIn)
					{
						// Point가 입력받은 Volume 내에 위치합니다.
						WriteFile << BoardPosition[0] << " " << BoardPosition[1] << " " << BoardPosition[2] << endl;
					}
				}
			}
			if (LinkedListPointer->GetNextNodePtr() != nullptr)
			{
				LinkedListPointer = LinkedListPointer->GetNextNodePtr();
			}
			else
			{
				break;
			}
		}
		WriteFile.close();
		FilterFile.close();
		delete[] FramePoint;
		delete[] dRayVector;
	}
	else
	{
		cout << "Scan Data가 존재하지 않습니다." << endl;
	}
}

/** ACheckerBoard Class의 정보를 기반으로 Checkerboard의 중심좌표 값을 Update 합니다. */
void AScanner::UpdateCenterPoint()
{
	double** CenterPointArray;
	AScannerHelper::SquareArrayDynamicAllocate(4, 1, CenterPointArray);

	double Width = HCheckerBoard->SquaresX * HCheckerBoard->SquareLength;
	double Height = HCheckerBoard->SquaresY * HCheckerBoard->SquareLength;

	CenterPointArray[0][0] = Width / 2.0f; // X axis
	CenterPointArray[1][0] = Height / 2.0f; // Y axis
	CenterPointArray[2][0] = 0.0f; // Z axis (Constrain)
	CenterPointArray[3][0] = 1.0f; // Define

	CenterPoint = AScannerHelper::ArrayToMat(4, 1, CenterPointArray).clone();

	AScannerHelper::SquareArrayAllocateFree(4, 1, CenterPointArray);
}

/** Pseudo Inverse의 결과를 인자로 받아서 현재 Checkerboard의 회전 중심좌표 값을 Update 합니다. */
void AScanner::UpdateOptimalPoint(Mat NewOptimalPoint)
{
	double** OptimalPointArray;
	AScannerHelper::SquareArrayDynamicAllocate(4, 1, OptimalPointArray);

	double* OptimalPointAccessor = NewOptimalPoint.ptr<double>(0);
	OptimalPointArray[0][0] = OptimalPointAccessor[3]; // X axis
	OptimalPointArray[1][0] = OptimalPointAccessor[4]; // Y axis
	OptimalPointArray[2][0] = 0.0f; // Z axis (Constrain)
	OptimalPointArray[3][0] = 1.0f; // Define

	OptimalPoint = AScannerHelper::ArrayToMat(4, 1, OptimalPointArray).clone();

	AScannerHelper::SquareArrayAllocateFree(4, 1, OptimalPointArray);
}

/** AScanner의 Linked List에 포함된 Data를 초기화합니다. */
void AScanner::ClearScanData()
{
	LinkedListHead->DestroyNode();
	LinkedListHead = new ALinkedList();
	bIsScanDataReady = false;
}

/** 현재 Object Checkerboard의 Yaw angle을 반환합니다. */
double AScanner::GetYawAngle()
{
	if (bIsValidPose)
	{
		Mat TransformB2C = ObjectCheckerBoard->GetTransformB2C();
		Mat RotationB2C = AScannerHelper::GetRotationMatrix(TransformB2C);
		return AScannerHelper::CalYaw(RotationB2C);
	}
	
	return 0.0f;
}