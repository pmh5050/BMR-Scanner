#include "AScannerHelper.h"

AScannerHelper::AScannerHelper()
{
}

AScannerHelper::~AScannerHelper()
{
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

/** PlaneParams와 RayVector를 입력받은 뒤, 교점을 계산하여 반환합니다.
@ PlaneParams의 경우 Line Laser Plane을 Camera Coordinate로 표현한 Parameter들에 해당합니다.
@ RayVector의 경우 Ray에 대한 방향 벡터를 Camera Coordinate로 표현한 Parameter들에 해당합니다.
@ 반환 값의 경우 마찬가지로 Camera Coordinate로 표현한 교점에 해당하며 이후 변환을 이용하여 복원에 사용됩니다.
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

/** 현재 Frame에 Gaussian Filter를 적용한 뒤, 이를 반환합니다 */
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

/** Image coordinate의 i와 j를 입력받아 계산한 Ray의 Vector를 RayVector에 기록합니다. */
void AScannerHelper::CalRayVector(int i, int j, AScanCamera* ScanCamera, double RayVector[3])
{
	RayVector[0] = (i*1.0f - ScanCamera->CenterOffset[0]) / (ScanCamera->FocalLength[0]); // (x - cx) / fx
	RayVector[1] = (j*1.0f - ScanCamera->CenterOffset[1]) / (ScanCamera->FocalLength[1]); // (y- cy) / fy
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

/** Point set을 입력받아 (x, y) 좌표계의 Equation Parameter를 반환합니다.
@ 반환 형식 : ay = bx + c 에서 Mat을 (b, a, c)로 구성하여 반환합니다.
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