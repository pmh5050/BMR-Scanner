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
	
	/** Mat�������� ���ο� Plane Parameter(Camera coordinate)�� �Է¹޾� �����մϴ�. */
	void SetPlaneParams(Mat NewPlaneParams);
	/** �ּҰ� �Ǵ� Red Threshold�� ���� ���������� �޾Ƽ� �����մϴ�. */
	void SetMinimumRedValueThreshold(int NewMinimumRedValueThreshold);

	/** Mat�������� ������ Plane Parameter�� ��ȯ�մϴ�. */
	Mat GetPlaneParams();
	/** ���������� ����� Red Threshold�� ���� ��ȯ�մϴ�. */
	int GetMinimumRedValueThreshold();

private:
	int MinimumRedValueThreshold;
	Mat PlaneParams; // ax + by + cz + d = 0 �� ǥ������ (a, b, c, d) Parameter�� ������ Matrix

};