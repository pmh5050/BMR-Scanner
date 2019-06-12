#pragma once

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;

class AScanDataSet
{
public:
	AScanDataSet();

	/** �ڽ��� Node�� �Ҹ��Ŵ */
	void DestroyNode();

	/** Accessor funtion */

	/** ���� Node�� Delta Angle ���� ��ȯ�մϴ� */
	float GetDeltaAngle();
	/** ���� Node�� Delta Time ���� ms ������ ��ȯ�մϴ� */
	int GetDeltaTimeMs();
	/** ���� Node�� ���� Image ���� Mat ���·� ��ȯ�մϴ� */
	Mat GetOrgImageData();
	/** ���� Node�� Delta Angle ���� �����մϴ� */
	void SetDeltaAngle(float NewDeltaAngle);
	/** ���� Node�� Delta Time ���� ms ������ �����մϴ� */
	void SetDeltaTimeMs(int NewDeltaTimeMs);
	/** ���� Node�� ���� Image ���� Mat ���·� �����մϴ� */
	void SetOrgImageData(Mat NewImageData);
	/** ���� Node�� B2C Transform Matrix�� Mat ���·� �����մϴ� */
	void SetTransformB2C(Mat NewTrasnformB2C);
	/** ���� Node�� B2C Transform Matrix�� Mat ���·� ��ȯ�մϴ� */
	Mat GetTransformB2C();
	/** ���� Node�� Image ������ ���� �ִ��� ������ ��ȯ�մϴ� */
	bool IsReadyImgData();
	/** ���� Node�� Image���� �νĵ� Marker�� Count�� ���� ���·� �����մϴ� */
	void SetDetectedMarkerCount(int NewDetectedMarkerCount);
	/** ���� Node�� Image���� �νĵ� Marker�� Count�� ���� ���·� ��ȯ�մϴ� */
	int GetDetectedMarkerCount();

	/** ���� Node���� Step motor�� Step�� Ƚ���� ��ȭ���� ���� ���·� �����մϴ� */
	void SetDeltaStepCount(int NewDeltaStepCount);
	/** ���� Node���� Step motor�� Step�� Ƚ���� ��ȭ���� ���� ���·� ��ȯ�մϴ� */
	int GetDeltaStepCount();

	/** ���� Node���� Frame�� Checkerboard�� ���� Pose�� ���������� �����ߴ� �� ���θ� �� ���·� �����մϴ� */
	void SetIsValidPose(bool NewIsValidPose);
	/** ���� Node���� Frame�� Checkerboard�� ���� Pose�� ���������� �����ߴ� �� ���θ� �� ���·� ��ȯ�մϴ� */
	bool GetIsValidPose();

private:
	float DeltaAngle; // ���� frame�� ȸ������ ������ ����
	int DeltaTimeMs; // ���� frame�� ����ð��� ������ ����
	Mat OrgImageData; // ���� frame�� Image ������ ������ ����
	bool bIsReadyImgData; // ���� Node�� Image ������ ���� �ִ��� ������ ������ ����
	Mat TransformB2C; // ���� Node�� Frame�� �����ִ� B2C Transform matrix
	int DetectedMarkerCount; // ���� Node�� Image���� �νĵ� Marker�� Count�� ������ ����
	int DeltaStepCount; // ���� Node���� Step motor�� Step�� Ƚ���� ��ȭ���� ������ ����
	bool bIsValidPose; // ���� Node���� Frame�� Checkerboard�� ���� Pose�� ���������� �����ߴ� �� ���θ� ������ ����
};