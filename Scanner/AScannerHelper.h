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

	/** ȸ�� Process�� ���������� �̷�������� ���θ� ��ȯ�մϴ�. */
	static bool IsRegressionCompleted();

	/** Math Helper funtion */

	/** ������ ���� �ش� ������ ����� ���ϵ��� Clipping �մϴ�. */
	static int Clipping(int Value, int Min, int Max);
	/** �� Vector�� �Է¹޾� ������ �մϴ�. */
	static Mat CalCrossProduct(Mat Vector1, Mat Vector2);
	/** 
	* PlaneParams�� RayVector�� �Է¹��� ��, ������ ����Ͽ� ��ȯ�մϴ�.
	* @param PlaneParams - Line Laser Plane�� Camera Coordinate�� ǥ���� Parameter�鿡 �ش��մϴ�.
	* @param RayVector - Ray�� ���� ���� ���͸� Camera Coordinate�� ǥ���� Parameter�鿡 �ش��մϴ�.
	* @return Camera Coordinate�� ǥ���� ������ �ش��ϸ� ���� ��ȯ�� �̿��Ͽ� ������ ���˴ϴ�.
	*/
	static Mat CalIntersectionPoint(Mat PlaneParams, Mat RayVector);
	/** ���� Frame�� Red Point�� ��ġ�� ����� ��, ������ �迭�� ����մϴ�. */
	static void CalcRedPoint(Mat Frame, int MinRedThreshold, int RedPointPosition[]);
	/** ���� Frame�� Gaussian Blur ó���� ��, Red Point�� ��ġ�� ����Ͽ� ������ �迭�� ����մϴ�. */
	static void CalcRedPointUseGaussianBlur(Mat Frame, double MinRedThreshold, int RedPointPosition[]);
	/** ���� Frame�� Gaussian Blur ó���� ��, Red Point�� ��ġ�� Pick��ġ�� �߽����� WindowLength��ŭ�� ��� ���� ���� �� ����Ͽ� ������ �迭�� ����մϴ�. */
	static void CalcRedPointUseGaussianBlur(Mat Frame, double MinRedThreshold, double RedPointPosition[], int WindowLength);
	/** Image coordinate�� Row�� Col�� �Է¹޾� ����� Ray�� Vector�� RayVector�� ����մϴ�. */
	static void CalRayVector(int Row, int Col, AScanCamera* ScanCamera, double RayVector[3]);
	/** Image coordinate�� Row�� Col�� �Ǽ������� �Է¹޾� ����� Ray�� Vector�� RayVector�� ����մϴ�. */
	static void CalRayFVector(double Row, double Col, AScanCamera* ScanCamera, double RayVector[3]);

	/** Red Point Position �迭�� �Է¹޾� �� �� ���� ���� �ִ� ���� ��ȯ�մϴ�. */
	static int CalNumberOfPoint(int RedPointPosition[]);
	/** 'Transform matrix'�� 'CheckerBoard's PlaneParams' �� AScanCamera Pointer�� Red Point�� Position���� �Է¹޾� �� Board�� ������ Line�� CheckerBoard Coordinate�� ��ȯ�մϴ�. */
	static Mat CalLinearRegression(Mat TransformMatrix, Mat PlaneParams, AScanCamera* ScanCamera, int RedPointPosition[]);
	/** ACheckerBoard�� AScanCamera�� ��ü �� Red Point�� Position���� �Է¹޾� �� Board�� ������ Line�� CheckerBoard Coordinate�� ��ȯ�մϴ�. */
	static Mat CalLinearRegression(ACheckerBoard* CheckerBoard, AScanCamera* ScanCamera, int RedPointPosition[]);
	/** CheckerBoard Coordinate�� ������ (X, Y)�� CheckerBoard�� �߽������� ���� �Է¹��� ��� ���ο� ��ġ�ϴ��� ���θ� ��ȯ�մϴ� */
	static bool IsInnerBoardBoundary(ACheckerBoard* CheckerBoard, double BoundSquareX, double BoundSquareY, double PositionX, double PositionY);
	/** Point�� Transform Matrix�� �̿��Ͽ�, ��ǥ�� ��ȯ�� �������ݴϴ�. */
	static Mat PointCoordinateTransform(Mat Point, Mat TransformMatrix);
	/** Line Vector�� Transform Matrix�� �̿��Ͽ�, Vector�� ���� ��ȯ�� �������ݴϴ� */
	static Mat LineVectorCoordinateTransform(Mat LineVector, Mat TransformMatrix);
	/** Frame�� MinimumRedthreshold ���� ���ڷ� �־ Line laser�� ǥ�õ� Frame�� ��ȯ�մϴ�. */
	static Mat GetLineLaserFrame(Mat Frame, int MinimumRedthreshold);
	/** 
	* Point set�� �Է¹޾� (x, y) ��ǥ���� Equation Parameter�� ��ȯ�մϴ�.
	* @return ay = bx + c ���� Mat�� (b, a, c)�� �����Ͽ� ��ȯ�մϴ�.
	*/
	static Mat PositionSetToLineEquation(double** PositionStack, int PoisitionStackPtr);
	/** y = ax + b ���� (a, 1, b)�� ���·� ����� Line Params�� x�� ���� �Է¹޾� Pivot Point (x0, y0, 0, 1)�� Mat ���·� ��ȯ�մϴ�. */
	static Mat CalLinePivotPoint(Mat LineParams, double PivotX);
	/** ���ڷ� ������ CheckerBoard�� 'CheckerBoard �߽� ��ǥ'���� '����(0, 0, 0)'������ ��ȯ ����� ��ȯ�մϴ�. */
	static Mat GetTransformCenterPointToOrigin(ACheckerBoard* CheckerBoard);
	/** LinkedList�� Head Pointer�� �Է¹޾� ��ü Sample�� ��ǥ�ϴ� ������ ȸ�� �߽� ��ǥ Vector (qx, qy, Qx, Qy, Qz)�� ��ȯ�մϴ�. */
	static Mat CalOptimalCenterPointVector(ALinkedList* LinkedListHead);
	/** ��ȯ�ϰ��� �ϴ� Frame�� ���ڷ� ���� ��, Red Data Frame�� ��ȯ�մϴ�. */
	static Mat CalRedFrame(Mat Frame);
	/** �Է¹��� Red Frame�� ���ؼ� Gaussian Blur�� Row ������ ������ ��, ��ȯ�մϴ�. */
	static Mat CalRowGaussianRedFrame(Mat RedFrame);
	/** Global optimazation ����� CenterPoint ����, ������ �Ǵ� Transform matrix(B2C)�� Update ����� Transform matrix(B2C)�� �Է¹��� ��, ����ȭ �� Transform matrix(C2O)�� ��ȯ�մϴ�. */
	static Mat CalOptimalTransformMatrix(Mat CenterPoint, Mat ObjectTransformMatrixB2C, Mat PivotTransformMatrixB2C);
	/** Pivot rotation matrix�� ���� Object rotation matrix������ Z�ุ�� ȸ�� ��ȯ ����� ��ȯ�մϴ�. */
	static Mat CalDeltaAngleMatrix(Mat PivotRotationMatrix, Mat ObjectRotationMatrix);
	/** �Է¹��� Rotation matrix�� Z�� ȸ����(Yaw)�� ����� ��, Radian ���·� ��ȯ���ݴϴ�. */
	static double CalYaw(Mat ObjectRotationMatrix);
	/** Transform matrix�� �Է¹޾� Rotation matrix�� ��ȯ�մϴ�. */
	static Mat GetRotationMatrix(Mat TransformMatrix);
	/** Transform matrix�� �Է¹޾� Translate matrix�� ��ȯ�մϴ�. */
	static Mat GetTranslateMatrix(Mat TransformMatrix);
	/** Rotation matrix�� Translate matrix�� �Է��Ͽ� Transform matrix�� ��ȯ�մϴ�. */
	static Mat GetTransformMatrix(Mat RotationMatrix, Mat TranslateMatrix);
	/** OffsetType ������ Position ������ ���� ��, Offset�� �ݿ��� ��ǥ�� ��ȯ�մϴ�. */
	static Mat GetOffsetCoordinate(EOffsetType OffsetType, Mat Position, Mat CenterPointCoordinate, Mat OptimalPointCoordinate);
	/** �迭�� �ּ� �� ���۰� �� Index�� �޾Ƽ� �ش� ���� ���� ��밪�� ��� �� ��ȯ�մϴ�. */
	static double GetMeanDataOfArray(double DataArray[], int InitIndex, int EndIndex);
	/** Plane Equation�� Parameter(ax + by + cz + d = 0)�� �迭 ���·� �޾Ƽ� Mat ���·� ��ȯ�մϴ�. */
	static Mat GetPlaneParameter(double PlaneParameterArray[4]);

	/** Constructor Helper funtion */

	/** Array�� Row, Col Size�� �Է¹޾� ����մϴ�. */
	template <typename TData>
	static void PrintArray(int ArrayRowSize, int ArrayColSize, TData** &Array);

	/** Array�� Row, Col Size�� �Է¹޾� Mat �������� ��ȯ�մϴ�. */
	template <typename TData>
	static Mat ArrayToMat(int ArrayRowSize, int ArrayColSize, TData** &TData);

	/** Array�� Row, Col Size�� �Է¹޾� Mat �������� ��ȯ�մϴ�. */
	template <typename TData>
	static Mat ArrayToMat(int ArrayRowSize, int ArrayColSize, TData* &Array);

	/** 
	* Mat�� Row, Col Size�� �Է¹޾� Array �������� ��ȯ�մϴ�.
	* @param Array - ���� �Ҵ��� ����� Array pointer �Դϴ�.
	*/
	template <typename TData>
	static void MatToArray(Mat ObjectMatrix, int ArrayRowSize, int ArrayColSize, TData** &Array);

	/** 2���� Array Pointer�� Row, Col Size�� �Է¹޾� �ش� �������� Array�� �����Ҵ� �մϴ�. */
	template <typename TData>
	static void SquareArrayDynamicAllocate(int ArrayRowSize, int ArrayColSize, TData** &Array);

	/** 2���� Array Pointer�� Row, Col Size�� �Է¹޾� �����Ҵ� �� Array�� �ݳ��մϴ�. */
	template <typename TData>
	static void SquareArrayAllocateFree(int ArrayRowSize, int ArrayColSize, TData** &Array);

private:
	static bool bIsRegressionCompleted;
};

/** Array�� Row, Col Size�� �Է¹޾� ����մϴ�. */
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

/** Array�� Row, Col Size�� �Է¹޾� Mat �������� ��ȯ�մϴ�. */
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

/** Array�� Row, Col Size�� �Է¹޾� Mat �������� ��ȯ�մϴ�. */
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
* Mat�� Row, Col Size�� �Է¹޾� Array �������� ��ȯ�մϴ�.
* @param Array - ���� �Ҵ��� ����� Array pointer �Դϴ�.
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

/** 2���� Array Pointer�� Row, Col Size�� �Է¹޾� �ش� �������� Array�� �����Ҵ� �մϴ�. */
template <typename TData>
void AScannerHelper::SquareArrayDynamicAllocate(int ArrayRowSize, int ArrayColSize, TData** &Array)
{
	Array = new TData*[ArrayRowSize];
	for (int Row = 0; Row < ArrayRowSize; Row++)
	{
		Array[Row] = new TData[ArrayColSize];
	}
}

/** 2���� Array Pointer�� Row, Col Size�� �Է¹޾� �����Ҵ� �� Array�� �ݳ��մϴ�. */
template <typename TData>
void AScannerHelper::SquareArrayAllocateFree(int ArrayRowSize, int ArrayColSize, TData** &Array)
{
	for (int Row = 0; Row < ArrayRowSize; Row++)
	{
		delete[] Array[Row];
	}
	delete[] Array;
}
