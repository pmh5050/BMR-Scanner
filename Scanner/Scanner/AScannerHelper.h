#pragma once

#include <iostream>
#include <math.h>

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include <AScanCamera.h>

#define WIDTH_INDEX 0
#define HEIGHT_INDEX 1

#define WIDTH_SIZE 640
#define HEIGHT_SIZE 480

using namespace std;
using namespace cv;

class AScannerHelper
{
public:
	AScannerHelper();

	~AScannerHelper();

	/** ������ ���� �ش� ������ ����� ���ϵ��� Clipping �մϴ�. */
	int Clipping(int Value, int Min, int Max);

	/** �� Vector�� �Է¹޾� ������ �մϴ�. */
	Mat CalCrossProduct(Mat Vector1, Mat Vector2);

	/** PlaneParams�� RayVector�� �Է¹��� ��, ������ ����Ͽ� ��ȯ�մϴ�.
	@ PlaneParams�� ��� Line Laser Plane�� Camera Coordinate�� ǥ���� Parameter�鿡 �ش��մϴ�.
	@ RayVector�� ��� Ray�� ���� ���� ���͸� Camera Coordinate�� ǥ���� Parameter�鿡 �ش��մϴ�.
	@ ��ȯ ���� ��� ���������� Camera Coordinate�� ǥ���� ������ �ش��ϸ� ���� ��ȯ�� �̿��Ͽ� ������ ���˴ϴ�.
	*/
	Mat CalIntersectionPoint(Mat PlaneParams, Mat RayVector);

	/** ���� Frame�� Gaussian Filter�� ������ ��, �̸� ��ȯ�մϴ� */
	void CalGaussianFrame(Mat Frame, int ImageSize[2], int FilterLength, double StandardDerivation);

	/** ���� Frame�� Red Point�� ��ġ�� ����� ��, ������ �迭�� ����մϴ�. */
	void CalcRedPoint(Mat Frame, int MinRedThreshold, int RedPointPosition[]);
	
	/** Image coordinate�� i�� j�� �Է¹޾� ����� Ray�� Vector�� RayVector�� ����մϴ�. */
	void CalRayVector(int i, int j, AScanCamera* ScanCamera, double RayVector[3]);

	/** Red Point Position �迭�� �Է¹޾� �� �� ���� ���� �ִ� ���� ��ȯ�մϴ�. */
	int CalNumberOfPoint(int RedPointPosition[]);

	/** 'Transform matrix'�� 'CheckerBoard's PlaneParams' �� AScanCamera Pointer�� Red Point�� Position���� �Է¹޾� �� Board�� ������ Line�� CheckerBoard Coordinate�� ��ȯ�մϴ�. */
	Mat CalLinearRegression(Mat TransformMatrix, Mat PlaneParams, AScanCamera* ScanCamera, int RedPointPosition[]);

	/** Point�� Transform Matrix�� �̿��Ͽ�, ��ǥ�� ��ȯ�� �������ݴϴ�. */
	Mat PointCoordinateTransform(Mat Point, Mat TransformMatrix);
	/** Line Vector�� Transform Matrix�� �̿��Ͽ�, Vector�� ���� ��ȯ�� �������ݴϴ� */
	Mat LineVectorCoordinateTransform(Mat LineVector, Mat TransformMatrix);
	/** Frame�� MinimumRedthreshold ���� ���ڷ� �־ Line laser�� ǥ�õ� Frame�� ��ȯ�մϴ�. */
	Mat GetLineLaserFrame(Mat Frame, int MinimumRedthreshold);

	/** Point set�� �Է¹޾� (x, y) ��ǥ���� Equation Parameter�� ��ȯ�մϴ�.
	@ ��ȯ ���� : ay = bx + c ���� Mat�� (b, a, c)�� �����Ͽ� ��ȯ�մϴ�.
	*/
	Mat PositionSetToLineEquation(double** PositionStack, int PoisitionStackPtr);

	/** y = ax + b ���� (a, 1, b)�� ���·� ����� Line Params�� x�� ���� �Է¹޾� Pivot Point (x0, y0, 0, 1)�� Mat ���·� ��ȯ�մϴ�. */
	Mat CalLinePivotPoint(Mat LineParams, double PivotX);

	bool bIsRegressionCompleted;
};

