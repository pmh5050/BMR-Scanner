#pragma once

#define CAMERA_INDEX 0

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
	~AScanCamera();

	/** Filename�� �Է¹޾� ī�޶��� ���� �Ķ���� �� �ְ� ����� �����մϴ� */
	void ReadCameraParameters(string Filename);

	bool IsFrameReady();
	bool GetFrame(Mat& Frame);
	void ShowFrame();
	
	VideoCapture Capture;

	// Camera Parameters
	Mat mCamMatrix; // ī�޶� ���� �Ķ����
	Mat mDistCoeffs; // ī�޶� �ܰ� ���

	bool bIsReadCameraParameters; // Camera parameter File�� �о�鿴���� ������ ����

	double CenterOffset[2];
	double FocalLength[2];
	// SVD

};

