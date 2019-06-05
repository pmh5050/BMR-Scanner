#pragma once

#include <iostream>
#include <math.h>

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include <ACheckerBoard.h>
#include <AScanCamera.h>
#include <ALinkedList.h>

#define WIDTH_INDEX 0
#define HEIGHT_INDEX 1

#define WIDTH_SIZE 1280 //640
#define HEIGHT_SIZE 960 //480

using namespace std;
using namespace cv;

enum EOffsetType
{
	None,
	CenterPoint,
	OptimalPoint
};

class AScannerHelper
{
public:
	AScannerHelper();

	/** Accessor funtion */

	/** 회귀 Process가 성공적으로 이루어졌는지 여부를 반환합니다. */
	static bool IsRegressionCompleted();

	/** Math Helper funtion */

	/** 변수의 값이 해당 범위를 벗어나지 못하도록 Clipping 합니다. */
	static int Clipping(int Value, int Min, int Max);
	/** 두 Vector를 입력받아 외적을 합니다. */
	static Mat CalCrossProduct(Mat Vector1, Mat Vector2);
	/** 
	* PlaneParams와 RayVector를 입력받은 뒤, 교점을 계산하여 반환합니다.
	* @param PlaneParams - Line Laser Plane을 Camera Coordinate로 표현한 Parameter들에 해당합니다.
	* @param RayVector - Ray에 대한 방향 벡터를 Camera Coordinate로 표현한 Parameter들에 해당합니다.
	* @return Camera Coordinate로 표현한 교점에 해당하며 이후 변환을 이용하여 복원에 사용됩니다.
	*/
	static Mat CalIntersectionPoint(Mat PlaneParams, Mat RayVector);
	/** 현재 Frame의 Red Point의 위치를 계산한 뒤, 제공된 배열에 기록합니다. */
	static void CalcRedPoint(Mat Frame, int MinRedThreshold, int RedPointPosition[]);
	/** 현재 Frame을 Gaussian Blur 처리한 뒤, Red Point의 위치를 계산하여 제공된 배열에 기록합니다. */
	static void CalcRedPointUseGaussianBlur(Mat Frame, double MinRedThreshold, int RedPointPosition[]);
	/** 현재 Frame을 Gaussian Blur 처리한 뒤, Red Point의 위치를 Pick위치를 중심으로 WindowLength만큼의 기대 값을 적용 후 계산하여 제공된 배열에 기록합니다. */
	static void CalcRedPointUseGaussianBlur(Mat Frame, double MinRedThreshold, double RedPointPosition[], int WindowLength);
	/** Image coordinate의 Row와 Col를 입력받아 계산한 Ray의 Vector를 RayVector에 기록합니다. */
	static void CalRayVector(int Row, int Col, AScanCamera* ScanCamera, double RayVector[3]);
	/** Image coordinate의 Row와 Col를 실수형으로 입력받아 계산한 Ray의 Vector를 RayVector에 기록합니다. */
	static void CalRayFVector(double Row, double Col, AScanCamera* ScanCamera, double RayVector[3]);

	/** Red Point Position 배열을 입력받아 총 몇 개의 점이 있는 지를 반환합니다. */
	static int CalNumberOfPoint(int RedPointPosition[]);
	/** 'Transform matrix'와 'CheckerBoard's PlaneParams' 및 AScanCamera Pointer와 Red Point의 Position들을 입력받아 각 Board에 투영된 Line을 CheckerBoard Coordinate로 반환합니다. */
	static Mat CalLinearRegression(Mat TransformMatrix, Mat PlaneParams, AScanCamera* ScanCamera, int RedPointPosition[]);
	/** ACheckerBoard와 AScanCamera의 객체 및 Red Point의 Position들을 입력받아 각 Board에 투영된 Line을 CheckerBoard Coordinate로 반환합니다. */
	static Mat CalLinearRegression(ACheckerBoard* CheckerBoard, AScanCamera* ScanCamera, int RedPointPosition[]);
	/** CheckerBoard Coordinate의 순서쌍 (X, Y)가 CheckerBoard의 중심점으로 부터 입력받은 경계 내부에 위치하는지 여부를 반환합니다 */
	static bool IsInnerBoardBoundary(ACheckerBoard* CheckerBoard, double BoundSquareX, double BoundSquareY, double PositionX, double PositionY);
	/** Point와 Transform Matrix를 이용하여, 좌표계 변환을 수행해줍니다. */
	static Mat PointCoordinateTransform(Mat Point, Mat TransformMatrix);
	/** Line Vector와 Transform Matrix를 이용하여, Vector에 대한 변환을 수행해줍니다 */
	static Mat LineVectorCoordinateTransform(Mat LineVector, Mat TransformMatrix);
	/** Frame과 MinimumRedthreshold 값을 인자로 주어서 Line laser만 표시된 Frame을 반환합니다. */
	static Mat GetLineLaserFrame(Mat Frame, int MinimumRedthreshold);
	/** 
	* Point set을 입력받아 (x, y) 좌표계의 Equation Parameter를 반환합니다.
	* @return ay = bx + c 에서 Mat을 (b, a, c)로 구성하여 반환합니다.
	*/
	static Mat PositionSetToLineEquation(double** PositionStack, int PoisitionStackPtr);
	/** y = ax + b 에서 (a, 1, b)의 형태로 저장된 Line Params와 x의 값을 입력받아 Pivot Point (x0, y0, 0, 1)를 Mat 형태로 반환합니다. */
	static Mat CalLinePivotPoint(Mat LineParams, double PivotX);
	/** 인자로 제공한 CheckerBoard의 'CheckerBoard 중심 좌표'에서 '원점(0, 0, 0)'으로의 변환 행렬을 반환합니다. */
	static Mat GetTransformCenterPointToOrigin(ACheckerBoard* CheckerBoard);
	/** LinkedList의 Head Pointer를 입력받아 전체 Sample을 대표하는 최적의 회전 중심 좌표 Vector (qx, qy, Qx, Qy, Qz)를 반환합니다. */
	static Mat CalOptimalCenterPointVector(ALinkedList* LinkedListHead);
	/** 변환하고자 하는 Frame을 인자로 받은 뒤, Red Data Frame을 반환합니다. */
	static Mat CalRedFrame(Mat Frame);
	/** 입력받은 Red Frame에 대해서 Gaussian Blur를 Row 단위로 적용한 뒤, 반환합니다. */
	static Mat CalRowGaussianRedFrame(Mat RedFrame);
	/** Global optimazation 결과인 CenterPoint 값과, 기준이 되는 Transform matrix(B2C)와 Update 대상인 Transform matrix(B2C)를 입력받은 뒤, 최적화 된 Transform matrix(C2O)를 반환합니다. */
	static Mat CalOptimalTransformMatrix(Mat CenterPoint, Mat ObjectTransformMatrixB2C, Mat PivotTransformMatrixB2C);
	/** Pivot rotation matrix로 부터 Object rotation matrix까지의 Z축만의 회전 변환 행렬을 반환합니다. */
	static Mat CalDeltaAngleMatrix(Mat PivotRotationMatrix, Mat ObjectRotationMatrix);
	/** 입력받은 Rotation matrix의 Z축 회전값(Yaw)을 계산한 뒤, Radian 형태로 반환해줍니다. */
	static double CalYaw(Mat ObjectRotationMatrix);
	/** Transform matrix를 입력받아 Rotation matrix를 반환합니다. */
	static Mat GetRotationMatrix(Mat TransformMatrix);
	/** Transform matrix를 입력받아 Translate matrix를 반환합니다. */
	static Mat GetTranslateMatrix(Mat TransformMatrix);
	/** Rotation matrix와 Translate matrix를 입력하여 Transform matrix를 반환합니다. */
	static Mat GetTransformMatrix(Mat RotationMatrix, Mat TranslateMatrix);
	/** OffsetType 정보와 Position 정보를 받은 뒤, Offset을 반영한 좌표를 반환합니다. */
	static Mat GetOffsetCoordinate(EOffsetType OffsetType, Mat Position, Mat CenterPointCoordinate, Mat OptimalPointCoordinate);
	/** 배열의 주소 및 시작과 끝 Index를 받아서 해당 범위 내의 기대값을 계산 후 반환합니다. */
	static double GetMeanDataOfArray(double DataArray[], int InitIndex, int EndIndex);
	/** Plane Equation의 Parameter(ax + by + cz + d = 0)를 배열 형태로 받아서 Mat 형태로 반환합니다. */
	static Mat GetPlaneParameter(double PlaneParameterArray[4]);

	/** Constructor Helper funtion */

	/** Array와 Row, Col Size를 입력받아 출력합니다. */
	template <typename TData>
	static void PrintArray(int ArrayRowSize, int ArrayColSize, TData** &Array);

	/** Array와 Row, Col Size를 입력받아 Mat 형식으로 반환합니다. */
	template <typename TData>
	static Mat ArrayToMat(int ArrayRowSize, int ArrayColSize, TData** &TData);

	/** Array와 Row, Col Size를 입력받아 Mat 형식으로 반환합니다. */
	template <typename TData>
	static Mat ArrayToMat(int ArrayRowSize, int ArrayColSize, TData* &Array);

	/** 
	* Mat와 Row, Col Size를 입력받아 Array 형식으로 반환합니다.
	* @param Array - 동적 할당이 선행된 Array pointer 입니다.
	*/
	template <typename TData>
	static void MatToArray(Mat ObjectMatrix, int ArrayRowSize, int ArrayColSize, TData** &Array);

	/** 2차원 Array Pointer와 Row, Col Size를 입력받아 해당 사이즈의 Array를 동적할당 합니다. */
	template <typename TData>
	static void SquareArrayDynamicAllocate(int ArrayRowSize, int ArrayColSize, TData** &Array);

	/** 2차원 Array Pointer와 Row, Col Size를 입력받아 동적할당 한 Array를 반납합니다. */
	template <typename TData>
	static void SquareArrayAllocateFree(int ArrayRowSize, int ArrayColSize, TData** &Array);

private:
	static bool bIsRegressionCompleted;
};

/** Array와 Row, Col Size를 입력받아 출력합니다. */
template <typename TData>
void AScannerHelper::PrintArray(int ArrayRowSize, int ArrayColSize, TData** &Array)
{
	for (int i = 0; i < ArrayRowSize; i++)
	{
		for (int j = 0; j < ArrayColSize; j++)
		{
			cout << Array[i][j] << " ";
		}
		cout << endl;
	}
}

/** Array와 Row, Col Size를 입력받아 Mat 형식으로 반환합니다. */
template <typename TData>
Mat AScannerHelper::ArrayToMat(int ArrayRowSize, int ArrayColSize, TData** &Array)
{
	TData* ListArrayClone = new TData[ArrayRowSize * ArrayColSize];
	for (int i = 0; i < ArrayRowSize; i++)
	{
		for (int j = 0; j < ArrayColSize; j++)
		{
			ListArrayClone[i * ArrayColSize + j] = Array[i][j];
		}
	}

	Mat ArrayClone;
	if (is_same<TData, double>::value == true)
	{
		ArrayClone = Mat(ArrayRowSize, ArrayColSize, CV_64F, ListArrayClone).clone();
	}
	else if(is_same<TData, int>::value == true)
	{
		ArrayClone = Mat(ArrayRowSize, ArrayColSize, CV_32S, ListArrayClone).clone();
	}
	else if (is_same<TData, unsigned char>::value == true)
	{
		ArrayClone = Mat(ArrayRowSize, ArrayColSize, CV_8U, ListArrayClone).clone();
	}

	delete[] ListArrayClone;

	return ArrayClone;
}

/** Array와 Row, Col Size를 입력받아 Mat 형식으로 반환합니다. */
template <typename TData>
Mat AScannerHelper::ArrayToMat(int ArrayRowSize, int ArrayColSize, TData* &Array)
{
	TData* ListArrayClone = new TData[ArrayRowSize * ArrayColSize];
	for (int i = 0; i < ArrayRowSize; i++)
	{
		for (int j = 0; j < ArrayColSize; j++)
		{
			ListArrayClone[i * ArrayColSize + j] = Array[i * ArrayColSize + j];
		}
	}

	Mat ArrayClone;
	if (is_same<TData, double>::value == true)
	{
		ArrayClone = Mat(ArrayRowSize, ArrayColSize, CV_64F, ListArrayClone).clone();
	}
	else if (is_same<TData, int>::value == true)
	{
		ArrayClone = Mat(ArrayRowSize, ArrayColSize, CV_32S, ListArrayClone).clone();
	}
	else if (is_same<TData, unsigned char>::value == true)
	{
		ArrayClone = Mat(ArrayRowSize, ArrayColSize, CV_8U, ListArrayClone).clone();
	}

	delete[] ListArrayClone;

	return ArrayClone;
}

/**
* Mat와 Row, Col Size를 입력받아 Array 형식으로 반환합니다.
* @param Array - 동적 할당이 선행된 Array pointer 입니다.
*/
template <typename TData>
void AScannerHelper::MatToArray(Mat ObjectMatrix, int ArrayRowSize, int ArrayColSize, TData** &Array)
{
	TData* MatrixAccessPointer;

	for (int Row = 0; Row < ArrayRowSize; Row++)
	{
		MatrixAccessPointer = ObjectMatrix.ptr<TData>(Row);
		for (int Col = 0; Col < ArrayColSize; Col++)
		{
			Array[Row][Col] = MatrixAccessPointer[Col];
		}
	}
}

/** 2차원 Array Pointer와 Row, Col Size를 입력받아 해당 사이즈의 Array를 동적할당 합니다. */
template <typename TData>
void AScannerHelper::SquareArrayDynamicAllocate(int ArrayRowSize, int ArrayColSize, TData** &Array)
{
	Array = new TData*[ArrayRowSize];
	for (int Row = 0; Row < ArrayRowSize; Row++)
	{
		Array[Row] = new TData[ArrayColSize];
	}
}

/** 2차원 Array Pointer와 Row, Col Size를 입력받아 동적할당 한 Array를 반납합니다. */
template <typename TData>
void AScannerHelper::SquareArrayAllocateFree(int ArrayRowSize, int ArrayColSize, TData** &Array)
{
	for (int Row = 0; Row < ArrayRowSize; Row++)
	{
		delete[] Array[Row];
	}
	delete[] Array;
}
