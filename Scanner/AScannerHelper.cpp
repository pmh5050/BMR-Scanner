#include "AScannerHelper.h"

#include "AScanDataSet.h"

#define ASCAANERHELPER_DEBUG false

bool AScannerHelper::bIsRegressionCompleted = false;

AScannerHelper::AScannerHelper()
{

}

/** ȸ�� Process�� ���������� �̷�������� ���θ� ��ȯ�մϴ�. */
bool AScannerHelper::IsRegressionCompleted()
{
	return AScannerHelper::bIsRegressionCompleted;
}

/** ������ ���� �ش� ������ ����� ���ϵ��� Clipping �մϴ�. */
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

/** �� Vector�� �Է¹޾� ������ �մϴ�. */
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
* PlaneParams�� RayVector�� �Է¹��� ��, ������ ����Ͽ� ��ȯ�մϴ�.
* @param PlaneParams - Line Laser Plane�� Camera Coordinate�� ǥ���� Parameter�鿡 �ش��մϴ�.
* @param RayVector - Ray�� ���� ���� ���͸� Camera Coordinate�� ǥ���� Parameter�鿡 �ش��մϴ�.
* @return Camera Coordinate�� ǥ���� ������ �ش��ϸ� ���� ��ȯ�� �̿��Ͽ� ������ ���˴ϴ�.
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

/** ���� Frame�� Red Point�� ��ġ�� ����� ��, ������ �迭�� ����մϴ�. */
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
			// Red point Ž�� ����
			RedPointPosition[i] = -1;
		}
		else
		{
			RedPointPosition[i] = MaxRedIndex;
		}
	}
}

/** ���� Frame�� Gaussian Blur ó���� ��, Red Point�� ��ġ�� ����Ͽ� ������ �迭�� ����մϴ�. */
void AScannerHelper::CalcRedPointUseGaussianBlur(Mat Frame, double MinRedThreshold, int RedPointPosition[])
{
	Mat RedFrame = AScannerHelper::CalRedFrame(Frame);
	Mat GaussianRedFrame = AScannerHelper::CalRowGaussianRedFrame(RedFrame);

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
			// Red point Ž�� ����
			RedPointPosition[i] = -1;
		}
		else
		{
			RedPointPosition[i] = MaxRedIndex;
		}
	}
}

/** Image coordinate�� Row�� Col�� �Է¹޾� ����� Ray�� Vector�� RayVector�� ����մϴ�. */
void AScannerHelper::CalRayVector(int Row, int Col, AScanCamera* ScanCamera, double RayVector[3])
{
	RayVector[0] = (Row*1.0f - ScanCamera->CenterOffset[0]) / (ScanCamera->FocalLength[0]); // (x - cx) / fx
	RayVector[1] = (Col*1.0f - ScanCamera->CenterOffset[1]) / (ScanCamera->FocalLength[1]); // (y- cy) / fy
	RayVector[2] = 1.0f;
}

/** Red Point Position �迭�� �Է¹޾� �� �� ���� ���� �ִ� ���� ��ȯ�մϴ�. */
int AScannerHelper::CalNumberOfPoint(int RedPointPosition[])
{
	int NumberOfRedPoint = 0;
	for (int i = 0; i < HEIGHT_SIZE; i++)
	{
		if (RedPointPosition[i] != -1) // �ش� Row�� Red Point�� �ִ� ���
		{
			NumberOfRedPoint++;
		}
	}
	return NumberOfRedPoint;
}

/** 'Transform matrix'�� 'CheckerBoard's PlaneParams' �� AScanCamera Pointer�� Red Point�� Position���� �Է¹޾� �� Board�� ������ Line�� CheckerBoard Coordinate�� ��ȯ�մϴ�. */
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
		if (RedPointPosition[i] != -1) // �ش� Row�� Red Point�� �ִ� ���
		{
			double dRayVector[3];
			CalRayVector(RedPointPosition[i], i, ScanCamera, dRayVector);
			Mat RayVector(1, 3, CV_64F, dRayVector);
			Mat Point = CalIntersectionPoint(PlaneParams, RayVector);
			Mat PointBoardPosition = PointCoordinateTransform(Point, InverseTransformMatrix);

			// ���� (Z = 0)
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
	// ����ȸ�� �� �ݳ�
	Mat LineVector = PositionSetToLineEquation(PointStack, PointStackPtr);
	for (int i = 0; i < NumberOfRedPoint; i++)
	{
		delete PointStack[i];
	}
	delete PointStack;

	return LineVector.clone();
}

/** ACheckerBoard�� AScanCamera�� ��ü �� Red Point�� Position���� �Է¹޾� �� Board�� ������ Line�� CheckerBoard Coordinate�� ��ȯ�մϴ�. */
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
		if (RedPointPosition[i] != -1) // �ش� Row�� Red Point�� �ִ� ���
		{
			double dRayVector[3];
			CalRayVector(RedPointPosition[i], i, ScanCamera, dRayVector);
			Mat RayVector(1, 3, CV_64F, dRayVector);
			Mat Point = CalIntersectionPoint(CheckerBoard->GetPlaneParams(), RayVector);
			Mat PointBoardPosition = PointCoordinateTransform(Point, InverseTransformMatrix);

			// ���� (Z = 0)
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
	// ����ȸ�� �� �ݳ�
	Mat LineVector = PositionSetToLineEquation(PointStack, PointStackPtr);
	for (int i = 0; i < NumberOfRedPoint; i++)
	{
		delete PointStack[i];
	}
	delete PointStack;

	return LineVector.clone();
}

/** CheckerBoard Coordinate�� ������ (X, Y)�� CheckerBoard�� �߽������� ���� �Է¹��� ��� ���ο� ��ġ�ϴ��� ���θ� ��ȯ�մϴ� */
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

/** Point�� Transform Matrix�� �̿��Ͽ�, ��ǥ�� ��ȯ�� �������ݴϴ�. */
Mat AScannerHelper::PointCoordinateTransform(Mat Point, Mat TransformMatrix)
{
	Mat ConvertedPoint = TransformMatrix * Point;

	return ConvertedPoint.clone();
}

/** Line Vector�� Transform Matrix�� �̿��Ͽ�, Vector�� ���� ��ȯ�� �������ݴϴ� */
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

/** Frame�� MinimumRedthreshold ���� ���ڷ� �־ Line laser�� ǥ�õ� Frame�� ��ȯ�մϴ�. */
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
* Point set�� �Է¹޾� (x, y) ��ǥ���� Equation Parameter�� ��ȯ�մϴ�.
* @return ay = bx + c ���� Mat�� (b, a, c)�� �����Ͽ� ��ȯ�մϴ�.
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

/** y = ax + b ���� (a, 1, b)�� ���·� ����� Line Params�� x�� ���� �Է¹޾� Pivot Point (x0, y0, 0, 1)�� Mat ���·� ��ȯ�մϴ�. */
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

/** ���ڷ� ������ CheckerBoard�� 'CheckerBoard �߽� ��ǥ'���� '����(0, 0, 0)'������ ��ȯ ����� ��ȯ�մϴ�. */
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

/** LinkedList�� Head Pointer�� �Է¹޾� ��ü Sample�� ��ǥ�ϴ� ������ ȸ�� �߽� ��ǥ Vector (qx, qy, Qx, Qy, Qz)�� ��ȯ�մϴ�. */
Mat AScannerHelper::CalOptimalCenterPointVector(ALinkedList* LinkedListHead)
{
	int LinkedListLength = LinkedListHead->GetLinkedListLength();
	double** dAMatrix;
	double* dBMatrix;

	// ���� �Ҵ�
	dAMatrix = new double*[LinkedListLength * 3];
	dBMatrix = new double[LinkedListLength * 3];
	for (int row = 0; row < LinkedListLength * 3; row++)
	{
		dAMatrix[row] = new double[5];
	}

#if ASCAANERHELPER_DEBUG
	cout << "���� �Ҵ� �Ϸ�" << endl;
#endif

	// Constrain : q(x, y, z) ���� z�� ���� �׻� 0�̹Ƿ�, A Matrix ���� 3���� ȿ���� ���� ������ �� ����
	ALinkedList* NodeLinkedList = LinkedListHead;
	AScanDataSet* NodeScanDataSet;
	Mat TransformMatrix;
	double* TransformPtr;
	for (int i = 0; i < LinkedListLength; i++)
	{
		for (int row = 0; row < 3; row++) // i : ��
		{
			NodeScanDataSet = NodeLinkedList->GetDataSetPtr();
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
		// Jump to Next Node
		NodeLinkedList = NodeLinkedList->GetNextNodePtr();
	}
	
	int RowSize = LinkedListLength * 3;
	
	Mat AMatrix = AScannerHelper::ArrayToMat(RowSize, 5, dAMatrix);
	Mat BMatrix = AScannerHelper::ArrayToMat(RowSize, 1, dBMatrix);
	
	Mat OptimalPointVector;

	solve(AMatrix, BMatrix, OptimalPointVector, DECOMP_SVD);
	
	// �ݳ�
	delete[] dBMatrix;

	for (int row = 0; row < LinkedListLength * 3; row++)
	{
		delete[] dAMatrix[row];
	}
	delete[] dAMatrix;

	return OptimalPointVector.clone();
}

/** ��ȯ�ϰ��� �ϴ� Frame�� ���ڷ� ���� ��, Red Data Frame�� ��ȯ�մϴ�. */
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

/** �Է¹��� Red Frame�� ���ؼ� Gaussian Blur�� Row ������ ������ ��, ��ȯ�մϴ�. */
Mat AScannerHelper::CalRowGaussianRedFrame(Mat RedFrame)
{
	const int KernalLength = 5;
	const double GaussianKernal[KernalLength] = { 1.0f, 4.0f, 6.0f, 4.0f, 1.0f };

	int KernalCenterIndex = KernalLength / 2 + 1;

	double KernalComponentSum = 0.0f; // Gaussian Kenral Component�� ���� ������ ����

	// Gaussian Kernal Component�� ��
	for (int i = 0; i < KernalLength; i++)
	{
		KernalComponentSum += GaussianKernal[i];
	}

	double** RedFrameArray; // Red Frame Data�� ������ Pointer
	AScannerHelper::SquareArrayDynamicAllocate(HEIGHT_SIZE, WIDTH_SIZE, RedFrameArray);
	AScannerHelper::MatToArray(RedFrame, HEIGHT_SIZE, WIDTH_SIZE, RedFrameArray);

	double** GaussianRedFrameArray; // Gaussian Blur�� ����� Array�� ���� ������ Pointer
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

/** Global optimazation ����� CenterPoint ����, ������ �Ǵ� Transform matrix(B2C)�� Update ����� Transform matrix(B2C)�� �Է¹��� ��, ����ȭ �� Transform matrix(C2O)�� ��ȯ�մϴ�. */
Mat AScannerHelper::CalOptimalTransformMatrix(Mat CenterPoint, Mat ObjectTransformMatrixB2C, Mat PivotTransformMatrixB2C)
{
	// ȸ�� �߽� ��ǥ�� Board Coordinate ������ Vector�� ����մϴ�.
	static double* CenterPointAccessor = CenterPoint.ptr<double>(0);
	static double dCenterPointInBoardCoordinate[3] = { CenterPointAccessor[3], CenterPointAccessor[4], 0.0f };
	static Mat CenterPointInBoardCoordinate = Mat(3, 1, CV_64F, dCenterPointInBoardCoordinate).clone();

	// Pivot Transform�� ���� ������ ����մϴ�.
	Mat PivotTranslateMatrix = AScannerHelper::GetTranslateMatrix(PivotTransformMatrixB2C.inv());
	Mat PivotRotationMatrix = AScannerHelper::GetRotationMatrix(PivotTransformMatrixB2C);
	
	Mat ObjectRotationMatrix = AScannerHelper::GetRotationMatrix(ObjectTransformMatrixB2C);
	
	Mat DeltaAngleRotationMatrix = AScannerHelper::CalDeltaAngleMatrix(PivotRotationMatrix, ObjectRotationMatrix);
	
	Mat OptimalTranslateMatrix = DeltaAngleRotationMatrix * (PivotTranslateMatrix - CenterPointInBoardCoordinate) + CenterPointInBoardCoordinate; // Camera to O
	Mat OptimalTransformMatrix = AScannerHelper::GetTransformMatrix(DeltaAngleRotationMatrix * PivotRotationMatrix.inv(), OptimalTranslateMatrix); // Camera to O

	return OptimalTransformMatrix.clone(); // Camera To O
}

/** Pivot rotation matrix�� ���� Object rotation matrix������ Z�ุ�� ȸ�� ��ȯ ����� ��ȯ�մϴ�. */
Mat AScannerHelper::CalDeltaAngleMatrix(Mat PivotRotationMatrix, Mat ObjectRotationMatrix)
{
	double** dDeltaRotationMatrix;
	AScannerHelper::SquareArrayDynamicAllocate(3, 3, dDeltaRotationMatrix);
	
	double PivotAngle = AScannerHelper::CalYaw(PivotRotationMatrix);
	double ObjectAngle = AScannerHelper::CalYaw(ObjectRotationMatrix);

	double DeltaAngle = ObjectAngle - PivotAngle;
	if (DeltaAngle < 0.0f)
	{
		DeltaAngle += 2 * CV_PI;
	}

	for (int Row = 0; Row < 3; Row++)
	{
		for (int Col = 0; Col < 3; Col++)
		{
			dDeltaRotationMatrix[Row][Col] = 0.0f;
		}
	}

	dDeltaRotationMatrix[0][0] = cos(DeltaAngle);
	dDeltaRotationMatrix[0][1] = -sin(DeltaAngle);
	dDeltaRotationMatrix[1][0] = sin(DeltaAngle);
	dDeltaRotationMatrix[1][1] = cos(DeltaAngle);
	
	dDeltaRotationMatrix[2][2] = 1.0f;
	Mat DeltaAngleRotationMatrix = AScannerHelper::ArrayToMat(3, 3, dDeltaRotationMatrix);
	AScannerHelper::SquareArrayAllocateFree(3, 3, dDeltaRotationMatrix);
	return DeltaAngleRotationMatrix.clone();
}

/** �Է¹��� Rotation matrix�� Z�� ȸ����(Yaw)�� ����� ��, Radian ���·� ��ȯ���ݴϴ�. */
double AScannerHelper::CalYaw(Mat ObjectRotationMatrix)
{
	// ObjectTransformMatrix�� ��� Board To Camera Transform matrix�� ����
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

/** Transform matrix�� �Է¹޾� Rotation matrix�� ��ȯ�մϴ�. */
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

/** Transform matrix�� �Է¹޾� Translate matrix�� ��ȯ�մϴ�. */
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

/** Rotation matrix�� Translate matrix�� �Է��Ͽ� Transform matrix�� ��ȯ�մϴ�. */
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

/** OffsetType ������ Position ������ ���� ��, Offset�� �ݿ��� ��ǥ�� ��ȯ�մϴ�. */
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