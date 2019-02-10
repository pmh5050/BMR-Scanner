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

	/** 변수의 값이 해당 범위를 벗어나지 못하도록 Clipping 합니다. */
	int Clipping(int Value, int Min, int Max);

	/** 두 Vector를 입력받아 외적을 합니다. */
	Mat CalCrossProduct(Mat Vector1, Mat Vector2);

	/** PlaneParams와 RayVector를 입력받은 뒤, 교점을 계산하여 반환합니다.
	@ PlaneParams의 경우 Line Laser Plane을 Camera Coordinate로 표현한 Parameter들에 해당합니다.
	@ RayVector의 경우 Ray에 대한 방향 벡터를 Camera Coordinate로 표현한 Parameter들에 해당합니다.
	@ 반환 값의 경우 마찬가지로 Camera Coordinate로 표현한 교점에 해당하며 이후 변환을 이용하여 복원에 사용됩니다.
	*/
	Mat CalIntersectionPoint(Mat PlaneParams, Mat RayVector);

	/** 현재 Frame에 Gaussian Filter를 적용한 뒤, 이를 반환합니다 */
	void CalGaussianFrame(Mat Frame, int ImageSize[2], int FilterLength, double StandardDerivation);

	/** 현재 Frame의 Red Point의 위치를 계산한 뒤, 제공된 배열에 기록합니다. */
	void CalcRedPoint(Mat Frame, int MinRedThreshold, int RedPointPosition[]);
	
	/** Image coordinate의 i와 j를 입력받아 계산한 Ray의 Vector를 RayVector에 기록합니다. */
	void CalRayVector(int i, int j, AScanCamera* ScanCamera, double RayVector[3]);

	/** Red Point Position 배열을 입력받아 총 몇 개의 점이 있는 지를 반환합니다. */
	int CalNumberOfPoint(int RedPointPosition[]);

	/** 'Transform matrix'와 'CheckerBoard's PlaneParams' 및 AScanCamera Pointer와 Red Point의 Position들을 입력받아 각 Board에 투영된 Line을 CheckerBoard Coordinate로 반환합니다. */
	Mat CalLinearRegression(Mat TransformMatrix, Mat PlaneParams, AScanCamera* ScanCamera, int RedPointPosition[]);

	/** Point와 Transform Matrix를 이용하여, 좌표계 변환을 수행해줍니다. */
	Mat PointCoordinateTransform(Mat Point, Mat TransformMatrix);
	/** Line Vector와 Transform Matrix를 이용하여, Vector에 대한 변환을 수행해줍니다 */
	Mat LineVectorCoordinateTransform(Mat LineVector, Mat TransformMatrix);
	/** Frame과 MinimumRedthreshold 값을 인자로 주어서 Line laser만 표시된 Frame을 반환합니다. */
	Mat GetLineLaserFrame(Mat Frame, int MinimumRedthreshold);

	/** Point set을 입력받아 (x, y) 좌표계의 Equation Parameter를 반환합니다.
	@ 반환 형식 : ay = bx + c 에서 Mat을 (b, a, c)로 구성하여 반환합니다.
	*/
	Mat PositionSetToLineEquation(double** PositionStack, int PoisitionStackPtr);

	/** y = ax + b 에서 (a, 1, b)의 형태로 저장된 Line Params와 x의 값을 입력받아 Pivot Point (x0, y0, 0, 1)를 Mat 형태로 반환합니다. */
	Mat CalLinePivotPoint(Mat LineParams, double PivotX);

	bool bIsRegressionCompleted;
};

