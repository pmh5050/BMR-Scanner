#pragma once

#include <iostream>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;

class AScanCamera
{
public:
	AScanCamera();

	/** Filename�� �Է¹޾� ī�޶��� ���� �Ķ���� �� �ְ� ����� �����մϴ� */
	void ReadCameraParameters(string Filename);

	/** Accessor funtion */

	/** ���� Camera�� Frame�� �غ� ���θ� ��ȯ�մϴ� */
	bool IsFrameReady();
	/** ���� Camera�� Frame�� ȹ���� ��, ���ڷ� ���� ��ü�� �ּҿ� ��ȯ�մϴ� */
	bool GetFrame(Mat& Frame);
	/** ���� Camera�� Frame�� ȹ�� �� ����մϴ� */
	void ShowFrame();
	
	// Camera Parameters
	Mat mCamMatrix; // ī�޶� ���� �Ķ����
	Mat mDistCoeffs; // ī�޶� �ܰ� ���
	double CenterOffset[2];
	double FocalLength[2];

private:
	VideoCapture Capture; // Camera module accessor ��ü
	bool bIsReadCameraParameters; // Camera parameter File�� �о�鿴���� ������ ����

};

