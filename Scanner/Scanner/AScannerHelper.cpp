#include "AScannerHelper.h"

AScannerHelper::AScannerHelper()
{
}

AScannerHelper::~AScannerHelper()
{
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

/** PlaneParams�� RayVector�� �Է¹��� ��, ������ ����Ͽ� ��ȯ�մϴ�.
@ PlaneParams�� ��� Line Laser Plane�� Camera Coordinate�� ǥ���� Parameter�鿡 �ش��մϴ�.
@ RayVector�� ��� Ray�� ���� ���� ���͸� Camera Coordinate�� ǥ���� Parameter�鿡 �ش��մϴ�.
@ ��ȯ ���� ��� ���������� Camera Coordinate�� ǥ���� ������ �ش��ϸ� ���� ��ȯ�� �̿��Ͽ� ������ ���˴ϴ�.
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

/** ���� Frame�� Gaussian Filter�� ������ ��, �̸� ��ȯ�մϴ� */
void AScannerHelper::CalGaussianFrame(Mat Frame, int ImageSize[2], int FilterLength, double StandardDerivation)
{
	Mat GaussianFrame(ImageSize[WIDTH_INDEX], ImageSize[HEIGHT_INDEX], CV_64F);
	int* GaussianFilter = new int[FilterLength];
	double FilterSum = 0.0f;
	// Make Gaussian filter (Kernal)
	for (int i = 0; i < FilterLength; i++)
	{
		double N = -(i * i);
		double D = 2 * StandardDerivation * StandardDerivation;
		GaussianFilter[i] = exp(N / D);
		FilterSum += GaussianFilter[i];
	}

	FilterSum = (FilterSum - GaussianFilter[0]) * 2 + GaussianFilter[0];

	// Calculation Gaussian filter
	for (int j = 0; j < ImageSize[HEIGHT_INDEX]; j++)
	{
		uchar* pointer_input = Frame.ptr<uchar>(j);
		uchar* pointer_ouput = GaussianFrame.ptr<uchar>(j);
		for (int i = 0; i < ImageSize[WIDTH_INDEX]; i++)
		{
			double GaussianValue[3] = { 0, 0, 0 };
			for (int k = 0; k < FilterLength; k++)
			{
				GaussianValue[0] = pointer_input[i * 3 + 0]; // Blue
				GaussianValue[1] = pointer_input[i * 3 + 1]; // Green
				GaussianValue[2] = pointer_input[i * 3 + 2]; // Red
			}

			pointer_ouput[i] = (GaussianValue[0]) / 3.0;
		}
	}

	delete GaussianFilter;
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

/** Image coordinate�� i�� j�� �Է¹޾� ����� Ray�� Vector�� RayVector�� ����մϴ�. */
void AScannerHelper::CalRayVector(int i, int j, AScanCamera* ScanCamera, double RayVector[3])
{
	RayVector[0] = (i*1.0f - ScanCamera->CenterOffset[0]) / (ScanCamera->FocalLength[0]); // (x - cx) / fx
	RayVector[1] = (j*1.0f - ScanCamera->CenterOffset[1]) / (ScanCamera->FocalLength[1]); // (y- cy) / fy
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
	CalcRedPoint(Frame, MinimumRedthreshold, PositionArray);

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

/** Point set�� �Է¹޾� (x, y) ��ǥ���� Equation Parameter�� ��ȯ�մϴ�.
@ ��ȯ ���� : ay = bx + c ���� Mat�� (b, a, c)�� �����Ͽ� ��ȯ�մϴ�.
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