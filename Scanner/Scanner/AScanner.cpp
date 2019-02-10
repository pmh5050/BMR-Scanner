#include <fstream>

#include "AScanner.h"

#include "AScanCamera.h"
#include "ACheckerBoard.h"
#include "ALineLaser.h"
#include "AScannerHelper.h"

#include "ALinkedList.h"
#include "AScanDataSet.h"

#define ASCANNER_DEBUG false

#define CAMERA_PARAMETER_FILE_PATH "LogCalibration.txt"
#define DETECTOR_PARAMETER_FILE_PATH "DetectorParameter.yml"

AScanner::AScanner(int argc, char **argv)
{
	Application = new QApplication(argc, argv);
	UserInterface = new AUserInterface();
	ScanCamera = new AScanCamera();
	HCheckerBoard = new ACheckerBoard(5, 7, 0.038f, 0.019f, 10);
	VCheckerBoard = new ACheckerBoard(5, 7, 0.038f, 0.019f, 6);
	ScannerHelper = new AScannerHelper();
	LineLaser = new ALineLaser();
	LinkedListHead = new ALinkedList();

	ScanCamera->ReadCameraParameters(CAMERA_PARAMETER_FILE_PATH);
	HCheckerBoard->ReadDetectorParameters(DETECTOR_PARAMETER_FILE_PATH);
	VCheckerBoard->ReadDetectorParameters(DETECTOR_PARAMETER_FILE_PATH);
	
	UserInterface->show();

	bIsValidPose = false; // �� �������� �ʱ�ȭ�˴ϴ�.
	bIsHLineReady = false;
	bIsVLineReady = false;
	bIsScanDataReady = false;
}


AScanner::~AScanner()
{

}

/** ������ ����� �Ǵ� CheckerBoard�� �����մϴ�.
@ 'H' : H Checkerboard�� ������ ����� �˴ϴ�.
@ 'V' : V Checkerboard�� ������ ����� �˴ϴ�.
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
		cout << "ERROR : �ش� CheckerBoard Type�� ��� �������� �ʽ��ϴ�." << endl;
		break;
	}
}

/** ������ ����� �Ǵ� ChekcerBoard�� Pointer�� ��ȯ�մϴ�. */
class ACheckerBoard* AScanner::GetObjectCheckerBoard()
{
	return ObjectCheckerBoard;
}

/** ObjectCheckerBoard�� ������ ��, member class�� �ش� ������ �����մϴ�. */
void AScanner::DetectCheckerBoard(Mat& Frame)
{
	bIsValidPose = false;

	ScanCamera->GetFrame(Frame);
	aruco::detectMarkers(Frame, ObjectCheckerBoard->Dictionary, MarkerCorners, MarkerIds, ObjectCheckerBoard->DetectorParams, RejectedMarkers);

	// ArUco marker�� 1�� �̻� ����Ǿ��� ���
	if (MarkerIds.size() > 0)
	{
		aruco::interpolateCornersCharuco(MarkerCorners, MarkerIds, Frame, ObjectCheckerBoard->Charucoboard, CharucoCorners, CharucoIds, ScanCamera->mCamMatrix, ScanCamera->mDistCoeffs);

		// Camera matrix�� ������ �˻��Ͽ� ũ�Ⱑ 0�� �ƴ��� Ȯ���մϴ�
		if (ScanCamera->mCamMatrix.total() != 0)
		{
			// ArUco �Լ��� ȣ���Ͽ� ��ȿ�� �������� ���θ� '�ڼ� ���� ���� ���� ����'�� �����մϴ�.
			// ���θ� Ȯ�ι޴� �Ͱ� ���ÿ� Rotation matrix�� Translate matrix�� �ּҸ� ���ڷ� �־ ���� �����մϴ�.
			bIsValidPose = aruco::estimatePoseCharucoBoard(CharucoCorners, CharucoIds, ObjectCheckerBoard->Charucoboard, ScanCamera->mCamMatrix, ScanCamera->mDistCoeffs, vRot, vTrs);
		}
	}
}

/** ObjectCheckerBoard�� Rotation matrix �� Translate matrix�� �̿��Ͽ� Transform maxtrix�� Update �մϴ�. */
void AScanner::UpdateCheckerBoardTransformMatrix()
{
	if (bIsValidPose)
	{
		double dTransformMatrix[4][4] = {0};
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

/** Frame�� �Է¹޾� ������ CheckerBoard�� ���� ǥ���մϴ�. */
void AScanner::DrawCheckerBoardAxis(Mat& Frame)
{
	// '�ڼ� ���� ���� ���� ����'�� ���� ���� ���
	if (bIsValidPose)
	{
		// ���� �׸��ϴ�
		aruco::drawAxis(Frame, ScanCamera->mCamMatrix, ScanCamera->mDistCoeffs, vRot, vTrs, ObjectCheckerBoard->AxisLength);
	}
}

/** CheckerBoard�� Normal Vector�� Pivot Point�� Camera Coordinate�� ���ؼ� Update �մϴ�. */
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

/** ���õ� CheckerBoard�� ������ ���� Line Update�� �����մϴ� */
void AScanner::UpdateCheckerBoardLaserLine(Mat& Frame)
{
	if (bIsValidPose)
	{
		int* RedPointPosition = new int[HEIGHT_SIZE];
		ScannerHelper->CalcRedPoint(Frame, LineLaser->GetMinimumRedValueThreshold(), RedPointPosition);

		Mat LineVector = ScannerHelper->CalLinearRegression(ObjectCheckerBoard->GetTransformB2C(), ObjectCheckerBoard->GetPlaneParams(), ScanCamera, RedPointPosition);
		if (ScannerHelper->bIsRegressionCompleted)
		{
			ObjectCheckerBoard->SetLaserLineVector(LineVector);
			if (ObjectCheckerBoard == HCheckerBoard) // ������ϰ�� Pivot Point�� �߰��� ����մϴ�
			{
				Mat PivotPoint = ScannerHelper->CalLinePivotPoint(LineVector, 0.0f);
#if ASCANNER_DEBUG
				cout << "Line Params : " << LineVector << endl;
				cout << "Pivot Point : " << PivotPoint << endl;
#endif
				ObjectCheckerBoard->SetLaserPivotPoint(PivotPoint);
				
				UserInterface->SetHLine(LineVector);
				bIsHLineReady = true;
			}
			else if(ObjectCheckerBoard == VCheckerBoard)
			{
				UserInterface->SetVLine(LineVector);
				bIsVLineReady = true;
			}
		}

		delete RedPointPosition;
	}
}

/** ���� Frame�� Pose�� �����ϴ��� ���θ� ��ȯ�մϴ� */
bool AScanner::IsValidPose()
{
	return bIsValidPose;
}

/** WindowHandler�� �̿��Ͽ� CurrentCursorData�� Update �մϴ� */
void AScanner::UpdateCursorData()
{
	WindowHandler = (HWND)cvGetWindowHandle("Frame");
	GetCursorPos(&CurrentCursorData);
	ScreenToClient(WindowHandler, &CurrentCursorData);
}

/** Checkerboard���� Line Params�� �̿��Ͽ�, Camera coordinate ���� Line laser�� Plane Params�� Update �մϴ�. */
void AScanner::UpdateLineLaserPlaneParams()
{
	if (bIsHLineReady && bIsVLineReady)
	{
		cout << "Update Line Laser Plane Parameters!" << endl;

		// Line Vector format : y = ax + b���� (a, 1, b)
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

		Mat cHLineVector = ScannerHelper->LineVectorCoordinateTransform(bHLineVector, HTransformMatrix);
		Mat cVLineVector = ScannerHelper->LineVectorCoordinateTransform(bVLineVector, VTransformMatrix);

		Mat cNormalVector = ScannerHelper->CalCrossProduct(cHLineVector, cVLineVector);
#if ASCANNER_DEBUG
		cout << "NormalVector in Camera Coordinate : " << cNormalVector << endl;
#endif

		Mat cPivotPoint = ScannerHelper->PointCoordinateTransform(bPivotPoint, HTransformMatrix);
		
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
			cout << "H Line ������ �ʿ��մϴ�." << endl;
		}
		if (!bIsVLineReady)
		{
			cout << "V Line ������ �ʿ��մϴ�." << endl;
		}
	}

	UserInterface->UserInterfaceAccessor.UpdateNoneBox->setChecked(true);
}

/** Scan data�� �����ϴ��� ���θ� ��ȯ�մϴ�. */
bool AScanner::IsScanDataReady()
{
	return bIsScanDataReady;
}

/** Scan�� �����մϴ�. */
void AScanner::ScanRunning()
{
	Mat CurrentFrame;
	ScanCamera->GetFrame(CurrentFrame);

	if (!bIsScanDataReady)
	{
		cout << "Scan" << endl;
		SetObjectCheckerBoard('H');
	}

	DetectCheckerBoard(CurrentFrame);

	if (bIsValidPose)
	{
		UpdateCheckerBoardTransformMatrix();

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
		ScanDataSet->SetOrgImageData(CurrentFrame);
		ScanDataSet->SetTransformB2C(TransformB2C);
	}
	else
	{
#if ASCANNER_DEBUG
		cout << "Fail for Board detect" << endl;
#endif
	}
}

/** Scan data�κ��� Point Cloud ������ ����� �� �ش� FileName�� �����մϴ� */
void AScanner::StorePointCloud(string FileName)
{
	ofstream WriteFile(FileName.data());
	if (!WriteFile.is_open())
	{
#if ASCANNER_DEBUG
		cout << "Data ����� ���� ������ open�� �� �����ϴ�." << endl;
#endif
	}
	if (bIsScanDataReady)
	{
#if ASCANNER_DEBUG
		cout << "Line Laser plane : "  << LineLaser->GetPlaneParams() << endl;
#endif
		// ����
		ALinkedList* LinkedListPointer = LinkedListHead;
		int* FramePoint = new int[HEIGHT_SIZE]; // �Ҵ�
		double* dRayVector = new double[3]; // �Ҵ�
		double* IntersectionPointPointer;
		double BoardPosition[3]; // x, y, z
		do
		{
			AScanDataSet* NodeScanDataSet = LinkedListPointer->GetDataSetPtr();
			Mat NodeFrame = NodeScanDataSet->GetOrgImageData();
			Mat GrayFrame = ScannerHelper->GetLineLaserFrame(NodeFrame, LineLaser->GetMinimumRedValueThreshold());
			

			imshow("Scan Node Frame", NodeFrame);
			imshow("Scan Laser Frame", GrayFrame);
			waitKey(10);

			ScannerHelper->CalcRedPoint(NodeFrame, LineLaser->GetMinimumRedValueThreshold(), FramePoint);

			for (int i = 0; i < HEIGHT_SIZE; i++)
			{
				if (FramePoint[i] != -1)
				{
					ScannerHelper->CalRayVector(FramePoint[i], i, ScanCamera, dRayVector);
					Mat TransformC2B = NodeScanDataSet->GetTransformB2C().inv();
					Mat RayVector(1, 3, CV_64F, dRayVector);
					Mat cIntersectionPoint = ScannerHelper->CalIntersectionPoint(LineLaser->GetPlaneParams(), RayVector);
					Mat bIntersectionPoint = ScannerHelper->PointCoordinateTransform(cIntersectionPoint, TransformC2B);

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
		cout << "Scan Data�� �������� �ʽ��ϴ�." << endl;
		WriteFile << "Scan Data doesn't exist" << endl;
	}
}

/** Scan data�κ��� Point Cloud ������ ����� �� �ش� FileName�� �����մϴ�, �߰��� ScanVolume�� �Է¹޾Ƽ� �ش� ������ ��� ��� Point���� �����մϴ�.
@ Scan Volume�� ���¿��� ù ��° index�� Axis�� �ش��ϸ�, �� ��° index�� ��� 0�̸� minimum, 1�̸� maximum�� �ش��մϴ�.
@ (����) : Scan Volume[0][0]�� ��� X Axis�� minimum ���� �ش��մϴ�.
*/
void AScanner::StorePointCloud(string FileName, double ScanVolume[3][2])
{
	ofstream WriteFile(FileName.data());
	if (!WriteFile.is_open())
	{
#if ASCANNER_DEBUG
		cout << "Data ����� ���� ������ open�� �� �����ϴ�." << endl;
#endif
	}
	if (bIsScanDataReady)
	{
#if ASCANNER_DEBUG
		cout << "Line Laser plane : " << LineLaser->GetPlaneParams() << endl;
#endif
		// ����
		ALinkedList* LinkedListPointer = LinkedListHead;
		int* FramePoint = new int[HEIGHT_SIZE]; // �Ҵ�
		double* dRayVector = new double[3]; // �Ҵ�
		double* IntersectionPointPointer;
		double BoardPosition[3]; // x, y, z
		do
		{
			AScanDataSet* NodeScanDataSet = LinkedListPointer->GetDataSetPtr();
			Mat NodeFrame = NodeScanDataSet->GetOrgImageData();
			Mat GrayFrame = ScannerHelper->GetLineLaserFrame(NodeFrame, LineLaser->GetMinimumRedValueThreshold());


			imshow("Scan Node Frame", NodeFrame);
			imshow("Scan Laser Frame", GrayFrame);
			waitKey(10);

			ScannerHelper->CalcRedPoint(NodeFrame, LineLaser->GetMinimumRedValueThreshold(), FramePoint);

			for (int i = 0; i < HEIGHT_SIZE; i++)
			{
				if (FramePoint[i] != -1)
				{
					ScannerHelper->CalRayVector(FramePoint[i], i, ScanCamera, dRayVector);
					Mat TransformC2B = NodeScanDataSet->GetTransformB2C().inv();
					Mat RayVector(1, 3, CV_64F, dRayVector);
					Mat cIntersectionPoint = ScannerHelper->CalIntersectionPoint(LineLaser->GetPlaneParams(), RayVector);
					Mat bIntersectionPoint = ScannerHelper->PointCoordinateTransform(cIntersectionPoint, TransformC2B);

#if ASCANNER_DEBUG
					cout << "cIntersection point [" << i << "] : " << cIntersectionPoint << endl;
					cout << "bIntersection point [" << i << "] : " << bIntersectionPoint << endl;
#endif
					IntersectionPointPointer = bIntersectionPoint.ptr<double>(0);
					for (int j = 0; j < 3; j++)
					{
						BoardPosition[j] = IntersectionPointPointer[j];
					}

					bool bIsVolumeIn = true;
					for (int j = 0; j < 3; j++)
					{
						if (ScanVolume[j][RANGE_MINIMUM] <= BoardPosition[j] && BoardPosition[j] <= ScanVolume[j][RANGE_MAXIMUM])
						{
							// �ش� Axis�� ������ �����մϴ�.
						}
						else
						{
							bIsVolumeIn = false;
						}
					}

					if (bIsVolumeIn)
					{
						// Point�� �Է¹��� Volume ���� ��ġ�մϴ�.
						WriteFile << BoardPosition[0] << " " << BoardPosition[1] << " " << BoardPosition[2] << endl;
					}
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
		cout << "Scan Data�� �������� �ʽ��ϴ�." << endl;
		WriteFile << "Scan Data doesn't exist" << endl;
	}
}