#pragma once

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;

class ALineLaser
{
public:
	ALineLaser();

	/** Accessor funtion */
	
	/** Mat형식으로 새로운 Plane Parameter(Camera coordinate)를 입력받아 저장합니다. */
	void SetPlaneParams(Mat NewPlaneParams);
	/** 최소가 되는 Red Threshold의 값을 정수형으로 받아서 저장합니다. */
	void SetMinimumRedValueThreshold(int NewMinimumRedValueThreshold);

	/** Mat형식으로 저장한 Plane Parameter를 반환합니다. */
	Mat GetPlaneParams();
	/** 정수형으로 저장된 Red Threshold의 값을 반환합니다. */
	int GetMinimumRedValueThreshold();

private:
	int MinimumRedValueThreshold;
	Mat PlaneParams; // ax + by + cz + d = 0 의 표현에서 (a, b, c, d) Parameter를 저장할 Matrix

};