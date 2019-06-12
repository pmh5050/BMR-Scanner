#include "AScannerHelper.h"

#include "AScanDataSet.h"

#define ASCAANERHELPER_DEBUG false

bool AScannerHelper::bIsRegressionCompleted = false;

AScannerHelper::AScannerHelper()
{

}

/** 회귀 Process가 성공적으로 이루어졌는지 여부를 반환합니다. */
bool AScannerHelper::IsRegressionCompleted()
{
	return AScannerHelper::bIsRegressionCompleted;
}

/** 변수의 값이 해당 범위를 벗어나지 못하도록 Clipping 합니다. */
int AScannerHelper::Clipping(int Value, int Min, int Max)
{
	if (Value < Min)
	{
		Value = Min;
	}
	else if (Value > Max)
	{
		Value = Max;
	}
	return Value;
}

/** 두 Vector를 입력받아 외적을 합니다. */
Mat AScannerHelper::CalCrossProduct(Mat Vector1, Mat Vector2)
{
	Mat CrossProductVector(3, 1, CV_64F);
	int Pointer[2];
	for (int i = 0; i < 3; i++)
	{
		Pointer[0] = (i + 1) % 3;
		Pointer[1] = (i + 2) % 3;
		CrossProductVector.at<double>(i) = Vector1.at<double>(Pointer[0]) * Vector2.at<double>(Pointer[1]) - Vector1.at<double>(Pointer[1]) * Vector2.at<double>(Pointer[0]);
	}
	return CrossProductVector;
}

/**
* PlaneParams와 RayVector를 입력받은 뒤, 교점을 계산하여 반환합니다.
* @param PlaneParams - Line Laser Plane을 Camera Coordinate로 표현한 Parameter들에 해당합니다.
* @param RayVector - Ray에 대한 방향 벡터를 Camera Coordinate로 표현한 Parameter들에 해당합니다.
* @return Camera Coordinate로 표현한 교점에 해당하며 이후 변환을 이용하여 복원에 사용됩니다.
*/
Mat AScannerHelper::CalIntersectionPoint(Mat PlaneParams, Mat RayVector)
{
	double* PlaneParamsPointer = PlaneParams.ptr<double>(0);
	double* RayVectorPointer = RayVector.ptr<double>(0);

	double N = -PlaneParamsPointer[3];
	double D = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		D = D + PlaneParamsPointer[i] * RayVectorPointer[i];
	}

	double RayGain = N / D;
	double dIntersectionPoint[4] = { 0, 0, 0, 1.0f };
	for (int i = 0; i < 3; i++)
	{
		dIntersectionPoint[i] = RayGain * RayVectorPointer[i];
	}

	Mat IntersectionPoint(4, 1, CV_64F, dIntersectionPoint);
	return IntersectionPoint.clone();
}

/** 현재 Frame의 Red Point의 위치를 계산한 뒤, 제공된 배열에 기록합니다. */
void AScannerHelper::CalcRedPoint(Mat Frame, int MinRedThreshold, int RedPointPosition[])
{
	for (int i = 0; i < HEIGHT_SIZE; i++)
	{
		int MaxRedValue = -255;
		int MaxRedIndex = 0;

		for (int j = 0; j < WIDTH_SIZE; j++)
		{
			uchar* FramePointer = Frame.ptr<uchar>(i);
			uchar B = FramePointer[j * 3 + 0];
			uchar G = FramePointer[j * 3 + 1];
			uchar R = FramePointer[j * 3 + 2];
			int RedValue = R - (G + B) / 2;
			if (RedValue > MaxRedValue)
			{
				MaxRedValue = RedValue;
				MaxRedIndex = j;
			}
		}
		if (MaxRedValue < MinRedThreshold)
		{
			// Red point 탐색 실패
			RedPointPosition[i] = -1;
		}
		else
		{
			RedPointPosition[i] = MaxRedIndex;
		}
	}
}

/** 현재 Frame을 Gaussian Blur 처리한 뒤, Red Point의 위치를 계산하여 제공된 배열에 기록합니다. */
void AScannerHelper::CalcRedPointUseGaussianBlur(Mat Frame, double MinRedThreshold, int RedPointPosition[])
{
	Mat RedFrame = AScannerHelper::CalRedFrame(Frame);
	Mat GaussianRedFrame = AScannerHelper::CalRowGaussianRedFrame(RedFrame);

	// Use Pick Value
	for (int i = 0; i < HEIGHT_SIZE; i++)
	{
		double MaxRedValue = -255.0f;
		int MaxRedIndex = 0;

		for (int j = 0; j < WIDTH_SIZE; j++)
		{
			double* FramePointer = GaussianRedFrame.ptr<double>(i);
			double RedValue = FramePointer[j];
			if (RedValue > MaxRedValue)
			{
				MaxRedValue = RedValue;
				MaxRedIndex = j;
			}
		}
		if (MaxRedValue < MinRedThreshold)
		{
			// Red point 탐색 실패
			RedPointPosition[i] = -1;
		}
		else
		{
			RedPointPosition[i] = MaxRedIndex;
		}
	}
}

/** 현재 Frame을 Gaussian Blur 처리한 뒤, Red Point의 위치를 Pick위치를 중심으로 WindowLength만큼의 기대 값을 적용 후 계산하여 제공된 배열에 기록합니다. */
void AScannerHelper::CalcRedPointUseGaussianBlur(Mat Frame, double MinRedThreshold, double RedPointPosition[], int WindowLength)
{
	Mat RedFrame = AScannerHelper::CalRedFrame(Frame);
	Mat GaussianRedFrame = AScannerHelper::CalRowGaussianRedFrame(RedFrame);

	bool bIsWindowLengthEvenNumber = (WindowLength % 2 == 0);
	int HalfOfWindowLength = WindowLength / 2;

	double* FramePointer;
	// Use Pick Value or Mean Value
	for (int i = 0; i < HEIGHT_SIZE; i++)
	{
		double MaxRedValue = -255.0f;
		int MaxRedIndex = 0;

		for (int j = 0; j < WIDTH_SIZE; j++)
		{
			FramePointer = GaussianRedFrame.ptr<double>(i);
			double RedValue = FramePointer[j];
			if (RedValue > MaxRedValue)
			{
				MaxRedValue = RedValue;
				MaxRedIndex = j;
			}
		}
		if (MaxRedValue < MinRedThreshold)
		{
			// Red point 탐색 실패
			RedPointPosition[i] = -1;
		}
		else
		{
			// RedPointPosition[i] = MaxRedIndex; // Direct

			// 평균 값을 가져옵니다.
			if (bIsWindowLengthEvenNumber)
			{
				int InitIndex = Clipping(MaxRedIndex - HalfOfWindowLength, 0, WIDTH_SIZE - 1);
				int EndIndex = Clipping(MaxRedIndex + HalfOfWindowLength, 0, WIDTH_SIZE - 1);
				RedPointPosition[i] = GetMeanDataOfArray(FramePointer, InitIndex, EndIndex);
			}
			else
			{
				int InitIndex = Clipping(MaxRedIndex - HalfOfWindowLength, 0, WIDTH_SIZE - 1);
				int EndIndex = Clipping(MaxRedIndex + HalfOfWindowLength + 1, 0, WIDTH_SIZE - 1);
				RedPointPosition[i] = GetMeanDataOfArray(FramePointer, InitIndex, EndIndex);
			}
		}
	}
}

/** Image coordinate의 Row와 Col를 입력받아 계산한 Ray의 Vector를 RayVector에 기록합니다. */
void AScannerHelper::CalRayVector(int Row, int Col, AScanCamera* ScanCamera, double RayVector[3])
{
	RayVector[0] = (Row*1.0f - ScanCamera->CenterOffset[0]) / (ScanCamera->FocalLength[0]); // (x - cx) / fx
	RayVector[1] = (Col*1.0f - ScanCamera->CenterOffset[1]) / (ScanCamera->FocalLength[1]); // (y- cy) / fy
	RayVector[2] = 1.0f;
}

/** Image coordinate의 Row와 Col를 실수형으로 입력받아 계산한 Ray의 Vector를 RayVector에 기록합니다. */
void AScannerHelper::CalRayFVector(double Row, double Col, AScanCamera* ScanCamera, double RayVector[3])
{
	RayVector[0] = (Row*1.0f - ScanCamera->CenterOffset[0]) / (ScanCamera->FocalLength[0]); // (x - cx) / fx
	RayVector[1] = (Col*1.0f - ScanCamera->CenterOffset[1]) / (ScanCamera->FocalLength[1]); // (y- cy) / fy
	RayVector[2] = 1.0f;
}

/** Red Point Position 배열을 입력받아 총 몇 개의 점이 있는 지를 반환합니다. */
int AScannerHelper::CalNumberOfPoint(int RedPointPosition[])
{
	int NumberOfRedPoint = 0;
	for (int i = 0; i < HEIGHT_SIZE; i++)
	{
		if (RedPointPosition[i] != -1) // 해당 Row에 Red Point가 있는 경우
		{
			NumberOfRedPoint++;
		}
	}
	return NumberOfRedPoint;
}

/** 'Transform matrix'와 'CheckerBoard's PlaneParams' 및 AScanCamera Pointer와 Red Point의 Position들을 입력받아 각 Board에 투영된 Line을 CheckerBoard Coordinate로 반환합니다. */
Mat AScannerHelper::CalLinearRegression(Mat TransformMatrix, Mat PlaneParams, AScanCamera* ScanCamera, int RedPointPosition[])
{
	bIsRegressionCompleted = false;

	Mat InverseTransformMatrix = TransformMatrix.inv();
	int NumberOfRedPoint = CalNumberOfPoint(RedPointPosition);
	double** PointStack = new double*[NumberOfRedPoint];
	int PointStackPtr = 0;
	for (int i = 0; i < NumberOfRedPoint; i++)
	{
		PointStack[i] = new double[2];
	}

	for (int i = 0; i < HEIGHT_SIZE; i++)
	{
		if (RedPointPosition[i] != -1) // 해당 Row에 Red Point가 있는 경우
		{
			double dRayVector[3];
			CalRayVector(RedPointPosition[i], i, ScanCamera, dRayVector);
			Mat RayVector(1, 3, CV_64F, dRayVector);
			Mat Point = CalIntersectionPoint(PlaneParams, RayVector);
			Mat PointBoardPosition = PointCoordinateTransform(Point, InverseTransformMatrix);

			// 연산 (Z = 0)
			double* PointBoardPositionPointer = PointBoardPosition.ptr<double>(0);
			double PositionX = PointBoardPositionPointer[0]; // X Position
			double PositionY = PointBoardPositionPointer[1];
			double PositionZ = PointBoardPositionPointer[2];

			if (abs(PositionZ) <= 0.01f)
			{
				if (0.038f <= PositionX && PositionX <= 0.152f)
				{
					if (0.076f <= PositionY && PositionY <= 0.228f)
					{
						PointStack[PointStackPtr][0] = PositionX;
						PointStack[PointStackPtr++][1] = PositionY;
					}
				}
			}
		}
	}

	if (PointStackPtr > 5)
	{
		cout << "Point Stack Ptr : " << PointStackPtr << endl;
		bIsRegressionCompleted = true;
	}
	// 선형회귀 및 반납
	Mat LineVector = PositionSetToLineEquation(PointStack, PointStackPtr);
	for (int i = 0; i < NumberOfRedPoint; i++)
	{
		delete PointStack[i];
	}
	delete PointStack;

	return LineVector.clone();
}

/** ACheckerBoard와 AScanCamera의 객체 및 Red Point의 Position들을 입력받아 각 Board에 투영된 Line을 CheckerBoard Coordinate로 반환합니다. */
Mat AScannerHelper::CalLinearRegression(ACheckerBoard* CheckerBoard, AScanCamera* ScanCamera, int RedPointPosition[])
{
	bIsRegressionCompleted = false;

	Mat InverseTransformMatrix = CheckerBoard->GetTransformB2C().inv();
	int NumberOfRedPoint = CalNumberOfPoint(RedPointPosition);
	double** PointStack = new double*[NumberOfRedPoint];
	int PointStackPtr = 0;
	for (int i = 0; i < NumberOfRedPoint; i++)
	{
		PointStack[i] = new double[2];
	}

	for (int i = 0; i < HEIGHT_SIZE; i++)
	{
		if (RedPointPosition[i] != -1) // 해당 Row에 Red Point가 있는 경우
		{
			double dRayVector[3];
			CalRayVector(RedPointPosition[i], i, ScanCamera, dRayVector);
			Mat RayVector(1, 3, CV_64F, dRayVector);
			Mat Point = CalIntersectionPoint(CheckerBoard->GetPlaneParams(), RayVector);
			Mat PointBoardPosition = PointCoordinateTransform(Point, InverseTransformMatrix);

			// 연산 (Z = 0)
			double* PointBoardPositionPointer = PointBoardPosition.ptr<double>(0);
			double PositionX = PointBoardPositionPointer[0]; // X Position
			double PositionY = PointBoardPositionPointer[1];
			double PositionZ = PointBoardPositionPointer[2];

			if (abs(PositionZ) <= 0.01f)
			{
				if(IsInnerBoardBoundary(CheckerBoard, 1.5, 1.5, PositionX, PositionY))
				{
					PointStack[PointStackPtr][0] = PositionX;
					PointStack[PointStackPtr++][1] = PositionY;
				}
			}
		}
	}

	if (PointStackPtr > 5)
	{
		cout << "Point Stack Ptr : " << PointStackPtr << endl;
		bIsRegressionCompleted = true;
	}
	// 선형회귀 및 반납
	Mat LineVector = PositionSetToLineEquation(PointStack, PointStackPtr);
	for (int i = 0; i < NumberOfRedPoint; i++)
	{
		delete PointStack[i];
	}
	delete PointStack;

	return LineVector.clone();
}

/** CheckerBoard Coordinate의 순서쌍 (X, Y)가 CheckerBoard의 중심점으로 부터 입력받은 경계 내부에 위치하는지 여부를 반환합니다 */
bool AScannerHelper::IsInnerBoardBoundary(ACheckerBoard* CheckerBoard, double BoundSquareX, double BoundSquareY, double PositionX, double PositionY)
{
	double CenterPointX = (CheckerBoard->SquaresX * CheckerBoard->SquareLength) / 2.0f;
	double CenterPointY = (CheckerBoard->SquaresY * CheckerBoard->SquareLength) / 2.0f;

	double BoundDeltaX = BoundSquareX * CheckerBoard->SquareLength;
	double BoundDeltaY = BoundSquareY * CheckerBoard->SquareLength;

	if (CenterPointX - BoundDeltaX <= PositionX && PositionX <= CenterPointX + BoundDeltaX)
	{
		if (CenterPointY - BoundDeltaY <= PositionY && PositionY <= CenterPointY + BoundDeltaY)
		{
			return true;
		}
	}
	
	return false;
}

/** Point와 Transform Matrix를 이용하여, 좌표계 변환을 수행해줍니다. */
Mat AScannerHelper::PointCoordinateTransform(Mat Point, Mat TransformMatrix)
{
	Mat ConvertedPoint = TransformMatrix * Point;

	return ConvertedPoint.clone();
}

/** Line Vector와 Transform Matrix를 이용하여, Vector에 대한 변환을 수행해줍니다 */
Mat AScannerHelper::LineVectorCoordinateTransform(Mat LineVector, Mat TransformMatrix)
{
	double* LineVectorPointer = LineVector.ptr<double>(0);
	double dColLineVector[4];
	for (int i = 0; i < 3; i++)
	{
		dColLineVector[i] = LineVectorPointer[i];
	}
	dColLineVector[3] = 0;

	Mat ColLineVector(4, 1, CV_64F, dColLineVector);
	Mat ConvertedColLineVector = TransformMatrix * ColLineVector;
	double* dConvertedColLineVectorPointer = ConvertedColLineVector.ptr<double>(0);
	double dConvertedLineVector[3];
	for (int i = 0; i < 3; i++)
	{
		dConvertedLineVector[i] = dConvertedColLineVectorPointer[i];
	}
	Mat ConvertedLineVector(1, 3, CV_64F, dConvertedLineVector);
	return ConvertedLineVector.clone();
}

/** Frame과 MinimumRedthreshold 값을 인자로 주어서 Line laser만 표시된 Frame을 반환합니다. */
Mat AScannerHelper::GetLineLaserFrame(Mat Frame, int MinimumRedthreshold)
{
	Mat LineLaserFrame(HEIGHT_SIZE, WIDTH_SIZE, CV_8U);
	int PositionArray[HEIGHT_SIZE];
	// CalcRedPoint(Frame, MinimumRedthreshold, PositionArray);
	CalcRedPointUseGaussianBlur(Frame, (double) MinimumRedthreshold, PositionArray);

	for (int i = 0; i < HEIGHT_SIZE; i++)
	{
		uchar* FramePointer = LineLaserFrame.ptr<uchar>(i);
		for (int j = 0; j < WIDTH_SIZE; j++)
		{
			if (PositionArray[i] == j)
			{
				FramePointer[j] = 255;
			}
			else
			{
				FramePointer[j] = 0;
			}
		}
	}
	return LineLaserFrame;
}

/**
* Point set을 입력받아 (x, y) 좌표계의 Equation Parameter를 반환합니다.
* @return ay = bx + c 에서 Mat을 (b, a, c)로 구성하여 반환합니다.
*/
Mat AScannerHelper::PositionSetToLineEquation(double** PositionStack, int PoisitionStackPtr)
{
	double SumX = 0;
	double SumY = 0;
	double SumXt2 = 0;
	double SumXY = 0;

	for (int i = 0; i < PoisitionStackPtr; i++)
	{
		SumX += PositionStack[i][0];
		SumY += PositionStack[i][1];
		SumXt2 += PositionStack[i][0] * PositionStack[i][0];
		SumXY += PositionStack[i][0] * PositionStack[i][1];
	}

	double Num = PoisitionStackPtr * SumXY - SumX * SumY;
	double Den = PoisitionStackPtr * SumXt2 - SumX * SumX;
	double XGain = Num / Den;
	double YGain = 1.0f;
	double Constant = (SumY / PoisitionStackPtr) - XGain * (SumX / PoisitionStackPtr);
	double dLineEquation[3] = { XGain, YGain, Constant };

	Mat LineEquation(1, 3, CV_64F, dLineEquation);
	return LineEquation.clone();
}

/** y = ax + b 에서 (a, 1, b)의 형태로 저장된 Line Params와 x의 값을 입력받아 Pivot Point (x0, y0, 0, 1)를 Mat 형태로 반환합니다. */
Mat AScannerHelper::CalLinePivotPoint(Mat LineParams, double PivotX)
{
	double* LineParamsPointer = LineParams.ptr<double>(0);
	double dLinePivotPoint[4];
	dLinePivotPoint[0] = PivotX;
	dLinePivotPoint[1] = LineParamsPointer[0] * PivotX + LineParamsPointer[2]; // y = ax + b
	dLinePivotPoint[2] = 0.0f;
	dLinePivotPoint[3] = 1.0f;

	Mat LinePivotPoint(4, 1, CV_64F, dLinePivotPoint);
	return LinePivotPoint.clone();
}

/** 인자로 제공한 CheckerBoard의 'CheckerBoard 중심 좌표'에서 '원점(0, 0, 0)'으로의 변환 행렬을 반환합니다. */
Mat AScannerHelper::GetTransformCenterPointToOrigin(ACheckerBoard* CheckerBoard)
{
	double CenterPoint[2];
	double dTransformMatrix[4][4];
	CheckerBoard->GetCenterPoint(CenterPoint);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				dTransformMatrix[i][j] = 1.0f;
			}
			else
			{
				dTransformMatrix[i][j] = 0.0f;
			}
		}
	}

	dTransformMatrix[0][3] = CenterPoint[0]; // X Translate
	dTransformMatrix[1][3] = CenterPoint[1]; // Y Translate

	Mat TransformMatrix(4, 4, CV_64F, dTransformMatrix);
	return TransformMatrix.clone();
}

/** LinkedList의 Head Pointer를 입력받아 전체 Sample을 대표하는 최적의 회전 중심 좌표 Vector (qx, qy, Qx, Qy, Qz)를 반환합니다. */
Mat AScannerHelper::CalOptimalCenterPointVector(ALinkedList* LinkedListHead)
{
	int LinkedListLength = LinkedListHead->GetLinkedListLength();
	double** dAMatrix;
	double* dBMatrix;

	// 동적 할당
	dAMatrix = new double*[LinkedListLength * 3];
	dBMatrix = new double[LinkedListLength * 3];
	for (int row = 0; row < LinkedListLength * 3; row++)
	{
		dAMatrix[row] = new double[5];
	}

#if ASCAANERHELPER_DEBUG
	cout << "동적 할당 완료" << endl;
#endif

	// Constrain : q(x, y, z) 에서 z의 값은 항상 0이므로, A Matrix 에서 3열은 효율을 위해 제외할 수 있음
	ALinkedList* NodeLinkedList = LinkedListHead;
	AScanDataSet* NodeScanDataSet;
	Mat TransformMatrix;
	double* TransformPtr;

	int PreviousStepCount = 0;
	for (int i = 0; i < LinkedListLength; i++)
	{
		for (int row = 0; row < 3; row++) // i : 행
		{
			NodeScanDataSet = NodeLinkedList->GetDataSetPtr();

			// Kalman Filter
			// int CurrentStepCount = NodeScanDataSet->GetStepCount();
			// int DeltaStepCount = CurrentStepCount - PreviousStepCount;
			// PreviousStepCount = CurrentStepCount;

			if (NodeScanDataSet->GetDetectedMarkerCount() < 5) // Odometry Data가 우세함
			{
				Mat DeltaAngleMatrix = AScannerHelper::GetTurntableDeltaRotMatrix(1);
				TransformMatrix = TransformMatrix * DeltaAngleMatrix;
				NodeScanDataSet->SetTransformB2C(TransformMatrix);
			}
			else // Measurement Data가 우세함
			{
				TransformMatrix = NodeScanDataSet->GetTransformB2C();
			}

			
			TransformPtr = TransformMatrix.ptr<double>(row);
			// A Matrix
			for (int col = 0; col < 5; col++)
			{
				if (col < 3) // Identity matrix
				{
					if (row == col)
					{
						dAMatrix[row + i * 3][col] = 1.0f;
					}
					else
					{
						dAMatrix[row + i * 3][col] = 0.0f;
					}
				}
				else // Rotation matrix
				{
					dAMatrix[row + i * 3][col] = -TransformPtr[col - 3];
				}
			}

			// B Matrix
			dBMatrix[row + i * 3] = TransformPtr[3];
		}
		// Jump to Next Node
		NodeLinkedList = NodeLinkedList->GetNextNodePtr();
	}
	
	int RowSize = LinkedListLength * 3;
	
	Mat AMatrix = AScannerHelper::ArrayToMat(RowSize, 5, dAMatrix);
	Mat BMatrix = AScannerHelper::ArrayToMat(RowSize, 1, dBMatrix);
	
	Mat OptimalPointVector;

	solve(AMatrix, BMatrix, OptimalPointVector, DECOMP_SVD);
	
	// 반납
	delete[] dBMatrix;

	for (int row = 0; row < LinkedListLength * 3; row++)
	{
		delete[] dAMatrix[row];
	}
	delete[] dAMatrix;

	return OptimalPointVector.clone();
}

/** LinkedList의 Head Pointer를 입력받아 Pose Data가 Valid한 Node Sample을 대표하는 최적의 회전 중심 좌표 Vector (qx, qy, Qx, Qy, Qz)를 반환합니다. */
Mat AScannerHelper::CalOptimalCenterPointVectorOfValidPoseNode(ALinkedList* LinkedListHead)
{
	int ValidPoseLinkedListLength = AScannerHelper::GetNumberOfValidPoseFrame(LinkedListHead);
	double** dAMatrix;
	double* dBMatrix;

	// 동적 할당
	dAMatrix = new double*[ValidPoseLinkedListLength * 3];
	dBMatrix = new double[ValidPoseLinkedListLength * 3];
	for (int row = 0; row < ValidPoseLinkedListLength * 3; row++)
	{
		dAMatrix[row] = new double[5];
	}

#if ASCAANERHELPER_DEBUG
	cout << "동적 할당 완료" << endl;
#endif

	// Constrain : q(x, y, z) 에서 z의 값은 항상 0이므로, A Matrix 에서 3열은 효율을 위해 제외할 수 있음
	ALinkedList* NodeLinkedList = LinkedListHead;
	AScanDataSet* NodeScanDataSet;
	Mat TransformMatrix;
	double* TransformPtr;

	int PreviousStepCount = 0;
	for (int i = 0; i < ValidPoseLinkedListLength; i++)
	{
		NodeScanDataSet = NodeLinkedList->GetDataSetPtr();

		// 현재 Node의 Frame이 Valid한 Pose를 갖는지 여부를 살핀 뒤
		if (NodeScanDataSet->GetIsValidPose())
		{
#if ASCAANERHELPER_DEBUG
			cout << "Valid Pose Node" << endl;
#endif
			// Valid한 경우 Global Optimization에 현재 Frame의 정보를 활용합니다.
			for (int row = 0; row < 3; row++) // i : 행
			{
				TransformMatrix = NodeScanDataSet->GetTransformB2C();
				TransformPtr = TransformMatrix.ptr<double>(row);
				// A Matrix
				for (int col = 0; col < 5; col++)
				{
					if (col < 3) // Identity matrix
					{
						if (row == col)
						{
							dAMatrix[row + i * 3][col] = 1.0f;
						}
						else
						{
							dAMatrix[row + i * 3][col] = 0.0f;
						}
					}
					else // Rotation matrix
					{
						dAMatrix[row + i * 3][col] = -TransformPtr[col - 3];
					}
				}

				// B Matrix
				dBMatrix[row + i * 3] = TransformPtr[3];
			}
		}
		else
		{
#if ASCAANERHELPER_DEBUG
			cout << "InValid Pose Node" << endl;
#endif
			// Valid하지 않은 경우 Global Optimization에 현재 Frame의 정보를 활용하지 않습니다.
			i = i - 1;
		}

		// Jump to Next Node
		NodeLinkedList = NodeLinkedList->GetNextNodePtr();
	}

	int RowSize = ValidPoseLinkedListLength * 3;

	Mat AMatrix = AScannerHelper::ArrayToMat(RowSize, 5, dAMatrix);
	Mat BMatrix = AScannerHelper::ArrayToMat(RowSize, 1, dBMatrix);

	Mat OptimalPointVector;

	solve(AMatrix, BMatrix, OptimalPointVector, DECOMP_SVD);

	// 반납
	delete[] dBMatrix;

	for (int row = 0; row < ValidPoseLinkedListLength * 3; row++)
	{
		delete[] dAMatrix[row];
	}
	delete[] dAMatrix;

	return OptimalPointVector.clone();

}

/** 변환하고자 하는 Frame을 인자로 받은 뒤, Red Data Frame을 반환합니다. */
Mat AScannerHelper::CalRedFrame(Mat Frame)
{
	double** RedFrameArray;

	AScannerHelper::SquareArrayDynamicAllocate(HEIGHT_SIZE, WIDTH_SIZE, RedFrameArray);

	for (int i = 0; i < HEIGHT_SIZE; i++)
	{
		for (int j = 0; j < WIDTH_SIZE; j++)
		{
			uchar* FramePointer = Frame.ptr<uchar>(i);
			double B = (double) FramePointer[j * 3 + 0];
			double G = (double) FramePointer[j * 3 + 1];
			double R = (double) FramePointer[j * 3 + 2];
			RedFrameArray[i][j] = R - (G + B) / 2.0f;
		}
	}

	Mat RedFrame = AScannerHelper::ArrayToMat(HEIGHT_SIZE, WIDTH_SIZE, RedFrameArray);
	AScannerHelper::SquareArrayAllocateFree(HEIGHT_SIZE, WIDTH_SIZE, RedFrameArray);

	return RedFrame.clone();
}

/** 입력받은 Red Frame에 대해서 Gaussian Blur를 Row 단위로 적용한 뒤, 반환합니다. */
Mat AScannerHelper::CalRowGaussianRedFrame(Mat RedFrame)
{
	const int KernalLength = 5;
	const double GaussianKernal[KernalLength] = { 1.0f, 4.0f, 6.0f, 4.0f, 1.0f };

	int KernalCenterIndex = KernalLength / 2 + 1;

	double KernalComponentSum = 0.0f; // Gaussian Kenral Component의 합을 저장할 변수

	// Gaussian Kernal Component의 합
	for (int i = 0; i < KernalLength; i++)
	{
		KernalComponentSum += GaussianKernal[i];
	}

	double** RedFrameArray; // Red Frame Data를 저장할 Pointer
	AScannerHelper::SquareArrayDynamicAllocate(HEIGHT_SIZE, WIDTH_SIZE, RedFrameArray);
	AScannerHelper::MatToArray(RedFrame, HEIGHT_SIZE, WIDTH_SIZE, RedFrameArray);

	double** GaussianRedFrameArray; // Gaussian Blur가 적용된 Array의 값을 저장할 Pointer
	AScannerHelper::SquareArrayDynamicAllocate(HEIGHT_SIZE, WIDTH_SIZE, GaussianRedFrameArray);

	for (int Row = 0; Row < HEIGHT_SIZE; Row++)
	{
		for (int Col = 0; Col < WIDTH_SIZE; Col++)
		{
			double CurrentGaussianBlurPixel = 0.0f;
			// Gaussian Bluring
			for (int i = 0; i < KernalLength; i++)
			{
				int CurrentRedFrameColIndex = (Col + i - KernalCenterIndex);
				if (0 <= CurrentRedFrameColIndex && CurrentRedFrameColIndex < WIDTH_SIZE)
				{
					CurrentGaussianBlurPixel = GaussianKernal[i] * RedFrameArray[Row][CurrentRedFrameColIndex];
				}
			}
			CurrentGaussianBlurPixel = CurrentGaussianBlurPixel / KernalComponentSum;
			// Bluring Data
			GaussianRedFrameArray[Row][Col] = CurrentGaussianBlurPixel;
		}
	}

	Mat GaussianRedFrame = ArrayToMat(HEIGHT_SIZE, WIDTH_SIZE, GaussianRedFrameArray);

	AScannerHelper::SquareArrayAllocateFree(HEIGHT_SIZE, WIDTH_SIZE, RedFrameArray);
	AScannerHelper::SquareArrayAllocateFree(HEIGHT_SIZE, WIDTH_SIZE, GaussianRedFrameArray);

	return GaussianRedFrame.clone();
}

/** Global optimazation 결과인 CenterPoint 값과, 기준이 되는 Transform matrix(B2C)와 Update 대상인 Transform matrix(B2C)를 입력받은 뒤, 최적화 된 Transform matrix(C2O)를 반환합니다. */
Mat AScannerHelper::CalOptimalTransformMatrix(Mat CenterPoint, Mat ObjectTransformMatrixB2C, Mat PivotTransformMatrixB2C)
{
	// 회전 중심 좌표를 Board Coordinate 형태의 Vector로 기록합니다.
	static double* CenterPointAccessor = CenterPoint.ptr<double>(0);
	static double dCenterPointInBoardCoordinate[3] = { CenterPointAccessor[3], CenterPointAccessor[4], 0.0f };
	static Mat CenterPointInBoardCoordinate = Mat(3, 1, CV_64F, dCenterPointInBoardCoordinate).clone();

	// Pivot Transform에 대한 정보를 기록합니다.
	Mat PivotTranslateMatrix = AScannerHelper::GetTranslateMatrix(PivotTransformMatrixB2C.inv());
	Mat PivotRotationMatrix = AScannerHelper::GetRotationMatrix(PivotTransformMatrixB2C);
	
	Mat ObjectRotationMatrix = AScannerHelper::GetRotationMatrix(ObjectTransformMatrixB2C);
	
	Mat DeltaAngleRotationMatrix = AScannerHelper::CalDeltaAngleMatrix(PivotRotationMatrix, ObjectRotationMatrix);
	
	Mat OptimalTranslateMatrix = DeltaAngleRotationMatrix * (PivotTranslateMatrix - CenterPointInBoardCoordinate) + CenterPointInBoardCoordinate; // Camera to O
	Mat OptimalTransformMatrix = AScannerHelper::GetTransformMatrix(DeltaAngleRotationMatrix * PivotRotationMatrix.inv(), OptimalTranslateMatrix); // Camera to O

	return OptimalTransformMatrix.clone(); // Camera To O
}

/** Global optimazation 결과인 CenterPoint 값과, 기준이 되는 Transform matrix(B2C)와 Scan Data set, Odometry & Measurement 특성 상수를 입력받은 뒤, Kalman Filter를 적용하여 최적화 된 Transform matrix(C2O)를 반환합니다. */
Mat AScannerHelper::CalOptimalTransformMatrixUseKF(Mat CenterPoint, class AScanDataSet* ScanDataSet, double OdometryCC, double MeasurementCC, Mat PivotTransformMatrixB2C)
{
	// 회전 중심 좌표를 Board Coordinate 형태의 Vector로 기록합니다.
	static double* CenterPointAccessor = CenterPoint.ptr<double>(0);
	static double dCenterPointInBoardCoordinate[3] = { CenterPointAccessor[3], CenterPointAccessor[4], 0.0f };
	static Mat CenterPointInBoardCoordinate = Mat(3, 1, CV_64F, dCenterPointInBoardCoordinate).clone();

	// Pivot Transform에 대한 정보를 기록합니다.
	Mat PivotTranslateMatrix = AScannerHelper::GetTranslateMatrix(PivotTransformMatrixB2C.inv());

	Mat PivotRotationMatrix = AScannerHelper::GetRotationMatrix(PivotTransformMatrixB2C);
	Mat ObjectRotationMatrix = AScannerHelper::GetRotationMatrix(ScanDataSet->GetTransformB2C());

	static double EstDeltaAngle = 0.0f; // 예측된 Delta angle
	static double EstError = 0.0f;
	double CurrentOdometrySTD = AScannerHelper::CalOdometrySTD(ScanDataSet->GetDeltaTimeMs(), OdometryCC);
	double CurrentMeasurementSTD;

	if (ScanDataSet->GetIsValidPose()) // Checkerboard가 검출되었을 경우
	{
		CurrentMeasurementSTD = AScannerHelper::CalMeasurementSTD(ScanDataSet->GetDetectedMarkerCount(), MeasurementCC);
	}
	else
	{
		CurrentMeasurementSTD = -1.0f;
	} 

	// Odometry Data
	EstDeltaAngle = EstDeltaAngle + ScanDataSet->GetDeltaStepCount() * CV_PI / 100.0f;
	EstError = EstError + CurrentOdometrySTD;

	double KalmanGain =  AScannerHelper::CalKalmanGain(CurrentOdometrySTD, CurrentMeasurementSTD);

	// Measurement Data
	Mat DeltaAngleRotationMatrix = AScannerHelper::CalDeltaAngleMatrix(PivotRotationMatrix, ObjectRotationMatrix);
	double MeasurmentDeltaAngle = AScannerHelper::CalRotationAngle(DeltaAngleRotationMatrix);

	EstDeltaAngle = EstDeltaAngle + KalmanGain * (MeasurmentDeltaAngle - EstDeltaAngle);
	EstError = (1.0f - KalmanGain) * EstError;

	Mat EstDeltaAngleRotationMatrix = AScannerHelper::GetYawMatrix(EstDeltaAngle);
#if ASCAANERHELPER_DEBUG
	cout << "Est delta angle : " << EstDeltaAngle << endl;
#endif

	ScanDataSet->SetDeltaAngle(EstDeltaAngle); // For Filtering Test

	Mat OptimalTranslateMatrix = EstDeltaAngleRotationMatrix * (PivotTranslateMatrix - CenterPointInBoardCoordinate) + CenterPointInBoardCoordinate; // Camera to O
	Mat OptimalTransformMatrix = AScannerHelper::GetTransformMatrix(EstDeltaAngleRotationMatrix * PivotRotationMatrix.inv(), OptimalTranslateMatrix); // Camera to O

	return OptimalTransformMatrix.clone(); // Camera To O
}

/** Pivot rotation matrix로 부터 Object rotation matrix까지의 Z축만의 회전 변환 행렬을 반환합니다. */
Mat AScannerHelper::CalDeltaAngleMatrix(Mat PivotRotationMatrix, Mat ObjectRotationMatrix)
{
	double PivotAngle = AScannerHelper::CalYaw(PivotRotationMatrix);
	double ObjectAngle = AScannerHelper::CalYaw(ObjectRotationMatrix);

	double DeltaAngle = ObjectAngle - PivotAngle;
	if (DeltaAngle < 0.0f)
	{
		DeltaAngle += 2 * CV_PI;
	}

	Mat DeltaAngleRotationMatrix = GetYawMatrix(DeltaAngle);
	return DeltaAngleRotationMatrix.clone();
}

/**
* 입력받은 Rotation matrix의 Z축 회전값(Yaw)을 계산한 뒤, Radian 형태로 반환해줍니다.
* @param ObjectRotationMatrix - TransformB2C의 Rotation matrix에 해당합니다.
*/
double AScannerHelper::CalYaw(Mat ObjectRotationMatrix)
{
	// ObjectTransformMatrix의 경우 Board To Camera Transform matrix라 가정
	double dUnitZ[3] = { 0.0f, 0.0f, 1.0f }; // Camera coordinate
	Mat UnitZ(3, 1, CV_64F, dUnitZ);

	Mat PivotVector = (ObjectRotationMatrix.inv()) * UnitZ;
	double* PivotVectorPointer = PivotVector.ptr<double>(0);
	double dPivotHorizontalVector[2];
	
	dPivotHorizontalVector[0] = PivotVectorPointer[0];
	dPivotHorizontalVector[1] = PivotVectorPointer[1];

	double CurrentYaw = atan2(dPivotHorizontalVector[1], dPivotHorizontalVector[0]) + CV_PI;

	return CurrentYaw;
}

/**
* 입력받은 Rotation matrix의 Z축 회전값(Yaw)을 계산한 뒤, Radian 형태로 반환해줍니다.
* @param ObjectRotationMatrix - Z축에 대한 회전 변환 행렬(3x3)입니다.
*/
double AScannerHelper::CalRotationAngle(Mat ObjectRotationMatrix)
{
	double* MatPointer = ObjectRotationMatrix.ptr<double>(0);
	double RotationAngle = atan2(-MatPointer[1], MatPointer[0]);
	return RotationAngle;
}

/** Transform matrix를 입력받아 Rotation matrix를 반환합니다. */
Mat AScannerHelper::GetRotationMatrix(Mat TransformMatrix)
{
	double** dTransformMatrix;
	double** dRotationMatrix;

	AScannerHelper::SquareArrayDynamicAllocate(4, 4, dTransformMatrix);
	AScannerHelper::SquareArrayDynamicAllocate(3, 3, dRotationMatrix);

	AScannerHelper::MatToArray(TransformMatrix, 4, 4, dTransformMatrix);

	for (int Row = 0; Row < 3; Row++)
	{
		for (int Col = 0; Col < 3; Col++)
		{
			dRotationMatrix[Row][Col] = dTransformMatrix[Row][Col];
		}
	}
	
	Mat RotationMatrix = AScannerHelper::ArrayToMat(3, 3, dTransformMatrix);

	AScannerHelper::SquareArrayAllocateFree(4, 4, dTransformMatrix);
	AScannerHelper::SquareArrayAllocateFree(3, 3, dRotationMatrix);

	return RotationMatrix.clone();
}

/** Transform matrix를 입력받아 Translate matrix를 반환합니다. */
Mat AScannerHelper::GetTranslateMatrix(Mat TransformMatrix)
{
	double** dTransformMatrix;
	double** dTranslateMatrix;

	AScannerHelper::SquareArrayDynamicAllocate(4, 4, dTransformMatrix);
	AScannerHelper::SquareArrayDynamicAllocate(3, 1, dTranslateMatrix);

	AScannerHelper::MatToArray(TransformMatrix, 4, 4, dTransformMatrix);

	int Col = 3;
	for (int Row = 0; Row < 3; Row++)
	{
		
		dTranslateMatrix[Row][0] = dTransformMatrix[Row][Col];
	}

	
	Mat TranslateMatrix = AScannerHelper::ArrayToMat(3, 1, dTranslateMatrix);

	AScannerHelper::SquareArrayAllocateFree(4, 4, dTransformMatrix);
	AScannerHelper::SquareArrayAllocateFree(3, 1, dTranslateMatrix);

	return TranslateMatrix.clone();
}

/** Rotation matrix와 Translate matrix를 입력하여 Transform matrix를 반환합니다. */
Mat AScannerHelper::GetTransformMatrix(Mat RotationMatrix, Mat TranslateMatrix)
{
	double** dTransformMatrix;
	double** dRotationMatrix;
	double** dTranslateMatrix;
	
	AScannerHelper::SquareArrayDynamicAllocate(4, 4, dTransformMatrix);
	AScannerHelper::SquareArrayDynamicAllocate(3, 3, dRotationMatrix);
	AScannerHelper::SquareArrayDynamicAllocate(3, 1, dTranslateMatrix);

	AScannerHelper::MatToArray(RotationMatrix, 3, 3, dRotationMatrix);
	AScannerHelper::MatToArray(TranslateMatrix, 3, 1, dTranslateMatrix);

	for (int Row = 0; Row < 3; Row++)
	{
		// Rotation matrix
		for (int Col = 0; Col < 3; Col++)
		{
			dTransformMatrix[Row][Col] = dRotationMatrix[Row][Col];
		}

		// Translate matrix
		dTransformMatrix[Row][3] = dTranslateMatrix[Row][0];
	}

	// Garbage value row vector
	for (int Col = 0; Col < 3; Col++)
	{
		dTransformMatrix[3][Col] = 0.0f;
	}
	dTransformMatrix[3][3] = 1.0f;

	Mat TransformMatrix = AScannerHelper::ArrayToMat(4, 4, dTransformMatrix);

	AScannerHelper::SquareArrayAllocateFree(4, 4, dTransformMatrix);
	AScannerHelper::SquareArrayAllocateFree(3, 3, dRotationMatrix);
	AScannerHelper::SquareArrayAllocateFree(3, 1, dTranslateMatrix);
	return TransformMatrix.clone();
}

/** OffsetType 정보와 Position 정보를 받은 뒤, Offset을 반영한 좌표를 반환합니다. */
Mat AScannerHelper::GetOffsetCoordinate(EOffsetType OffsetType, Mat Position, Mat CenterPointCoordinate, Mat OptimalPointCoordinate)
{
	Mat OffsetCoordinate;
	switch (OffsetType)
	{
	case EOffsetType::CenterPoint:
		OffsetCoordinate = Position - CenterPointCoordinate;
		break;
	case EOffsetType::OptimalPoint:
		OffsetCoordinate = Position - OptimalPointCoordinate;
		break;
	default:
		OffsetCoordinate = Position;
		break;
	}
	return OffsetCoordinate.clone();
}

/** 배열의 주소 및 시작과 끝 Index를 받아서 해당 범위 내의 기대값을 계산 후 반환합니다. */
double AScannerHelper::GetMeanDataOfArray(double DataArray[], int InitIndex, int EndIndex)
{
	int DataLenght = EndIndex - InitIndex + 1;
	double Expectation = 0.0f;

	double Num = 0.0f; // 분자
	double Den = 0.0f; // 분모
#if ASCAANERHELPER_DEBUG
	if (DataLenght < 0)
	{
		cout << "Data length가 0보다 작습니다." << endl;
	}
#endif
	for (int Index = InitIndex; Index <= EndIndex; Index++)
	{
		Num += DataArray[Index] * Index;
		Den += DataArray[Index];
	}
	
	Expectation = Num / Den;
	return Expectation;
}

/** Plane Equation의 Parameter(ax + by + cz + d = 0)를 배열 형태로 받아서 Mat 형태로 반환합니다. */
Mat AScannerHelper::GetPlaneParameter(double PlaneParameterArray[4])
{
	Mat PlaneParameter(1, 4, CV_64F, PlaneParameterArray);
	return PlaneParameter.clone();
}

/** Step Count를 입력받아 Turntable의 Delta Rotation Matrix를 Mat 형태로 반환합니다. */
Mat AScannerHelper::GetTurntableDeltaRotMatrix(int StepCount)
{
	const bool bIsCCWDirection = true;
	const float DeltaAnglePerOneStep = 1.8f;
	double** DeltaRotationArray;
	
	AScannerHelper::SquareArrayDynamicAllocate(4, 4, DeltaRotationArray);
	
	for (int Row = 0; Row < 4; Row++)
	{
		for (int Col = 0; Col < 4; Col++)
		{
			DeltaRotationArray[Row][Col] = 0.0f;
		}
	}
	
	DeltaRotationArray[0][0] = cos(DeltaAnglePerOneStep * StepCount);
	DeltaRotationArray[0][1] = -sin(DeltaAnglePerOneStep * StepCount);
	DeltaRotationArray[1][0] = sin(DeltaAnglePerOneStep * StepCount);
	DeltaRotationArray[1][1] = cos(DeltaAnglePerOneStep * StepCount);
	DeltaRotationArray[2][2] = 1.0f;

	DeltaRotationArray[3][3] = 1.0f;

	Mat TurntableDeltaRotMatrix = ArrayToMat(4, 4, DeltaRotationArray);
	AScannerHelper::SquareArrayAllocateFree(4, 4, DeltaRotationArray);

	return TurntableDeltaRotMatrix;
}

/** Linked List에 포함된 Frame 중 Pose가 Valid한 Frame의 개수를 정수형태로 반환합니다. */
int AScannerHelper::GetNumberOfValidPoseFrame(ALinkedList* LinkedList)
{
	int NumberOfValidPoseFrame = 0;
	ALinkedList* CurrentLinkedList = LinkedList;
	while (true)
	{
		AScanDataSet* ScanDataSet = CurrentLinkedList->GetDataSetPtr();
		if (ScanDataSet->GetIsValidPose())
		{
			NumberOfValidPoseFrame++;
		}
		CurrentLinkedList = CurrentLinkedList->GetNextNodePtr();
		if (CurrentLinkedList == nullptr)
		{
			break;
		}
	}

	return NumberOfValidPoseFrame;

}

/** Yaw angle을 radian으로 입력받아 Z축에 대한 회전 변환 행렬을 만듭니다. */
Mat AScannerHelper::GetYawMatrix(double YawAngle)
{
	double** dDeltaRotationMatrix;
	AScannerHelper::SquareArrayDynamicAllocate(3, 3, dDeltaRotationMatrix);

	if (YawAngle < 0.0f)
	{
		YawAngle += 2 * CV_PI;
	}

	for (int Row = 0; Row < 3; Row++)
	{
		for (int Col = 0; Col < 3; Col++)
		{
			dDeltaRotationMatrix[Row][Col] = 0.0f;
		}
	}

	dDeltaRotationMatrix[0][0] = cos(YawAngle);
	dDeltaRotationMatrix[0][1] = -sin(YawAngle);
	dDeltaRotationMatrix[1][0] = sin(YawAngle);
	dDeltaRotationMatrix[1][1] = cos(YawAngle);

	dDeltaRotationMatrix[2][2] = 1.0f;

	Mat DeltaAngleRotationMatrix = AScannerHelper::ArrayToMat(3, 3, dDeltaRotationMatrix);
	AScannerHelper::SquareArrayAllocateFree(3, 3, dDeltaRotationMatrix);
	return DeltaAngleRotationMatrix.clone();
}

/** 회전 명령을 한 뒤의 경과 시간과 Odometry 특성 상수 값을 입력받아 해당하는 Odometry model의 표준편차 값을 반환합니다. */
double AScannerHelper::CalOdometrySTD(int DeltaTime, double OdometryCC)
{
	const double SettlingTimeSTD = 0.1f;
	const int SettlingTime = 700;

	double DecayPower = - (OdometryCC * (DeltaTime - SettlingTime)) / (double) SettlingTime;
	return SettlingTimeSTD * exp(DecayPower);
}

/** Frame에 검출된 Marker의 개수를 입력받아 해당하는 Measurement model의 표준편차 값을 반환합니다. */
double AScannerHelper::CalMeasurementSTD(int DetectedMarker, double MeasurementCC)
{
	const double ThresholdSTD = 0.05f;
	const int ThresholdMarkerNumber = 5;

	if (DetectedMarker >= ThresholdMarkerNumber)
	{
		double DecayPower = - (MeasurementCC * (DetectedMarker - ThresholdMarkerNumber)) / (double) ThresholdMarkerNumber;
		return ThresholdSTD * exp(DecayPower);
	}
	else
	{
		return -1.0f;
	}
}

/** Odometry 표준편차와 Measurement 표준편차를 입력받아 kalman Gain을 반환합니다. */
double AScannerHelper::CalKalmanGain(double OdometrySTD, double MeasurementSTD)
{
	double KalmanGain;
	if (MeasurementSTD > 0.0f) // Measurement data가 측정된 경우
	{
		KalmanGain = OdometrySTD / (OdometrySTD + MeasurementSTD);

	}
	else // Measurement data가 측정되지 않은 경우
	{
#if ASCAANERHELPER_DEBUG
		cout << "Mea STD is Infinity Value" << endl;
#endif
		KalmanGain = 0.0f;
	}

	return KalmanGain;
}