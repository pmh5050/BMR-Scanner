#pragma once

#define ASCANDATASET_DEBUG false

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;

class AScanDataSet
{
public:
	AScanDataSet();
	~AScanDataSet();

	/** �ڽ��� Node�� �Ҹ��Ŵ */
	void DestroyNode();

	/** Accessor �Լ� */
	float GetDeltaAngle();
	int GetDeltaTimeMs();
	Mat GetOrgImageData();

	void SetDeltaAngle(float NewDeltaAngle);
	void SetDeltaTimeMs(int NewDeltaTimeMs);
	void SetOrgImageData(Mat NewImageData);
	/** Mat �������� B2C Transform Matrix�� �޾Ƽ� �����մϴ� */
	void SetTransformB2C(Mat NewTrasnformB2C);
	/** Mat �������� B2C Transform Matrix�� ��ȯ�մϴ� */
	Mat GetTransformB2C();
	
	/** ���� Node�� Image ������ ���� �ִ��� ������ ��ȯ�մϴ� */
	bool IsReadyImgData();

private:
	/** ���� frame�� ȸ������ ������ ���� */
	float DeltaAngle;
	/** ���� frame�� ����ð��� ������ ���� */
	int DeltaTimeMs;
	/** ���� frame�� Image ������ ������ ���� */
	Mat OrgImageData;
	/** ���� Node�� Image ������ ���� �ִ��� ������ ������ ���� */
	bool bIsReadyImgData;
	/** ���� Node�� Frame�� �����ִ� B2C Transform matrix */
	Mat TransformB2C; // CheckerBoard Coordinate�� Camera Coordinate�� ��ȯ�ϴ� Transform matrix
};