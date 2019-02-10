#pragma once

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;

class ALineLaser
{
public:
	ALineLaser();
	~ALineLaser();

	/** Mat�������� ���ο� Plane Parameter�� �Է¹޾� �����մϴ�. */
	void SetPlaneParams(Mat NewPlaneParams);
	/** Mat�������� ������ Plane Parameter�� ��ȯ�մϴ�. */
	Mat GetPlaneParams();
	/** �ּҰ� �Ǵ� Red Threshold�� ���� ���������� �޾Ƽ� �����մϴ�. */
	void SetMinimumRedValueThreshold(int NewMinimumRedValueThreshold);
	/** ���������� ����� Red Threshold�� ���� ��ȯ�մϴ�. */
	int GetMinimumRedValueThreshold();

	Mat PlaneParams; // ax + by + cz + d = 0 �� ǥ������ (a, b, c, d) Parameter�� ������ Matrix

	int MinimumRedValueThreshold;
};