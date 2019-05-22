#pragma once

#define CAMERA_INDEX 0

#include <iostream>

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include "FlyCapture2.h"

using namespace std;
using namespace cv;

using namespace FlyCapture2;

enum ECameraType {
	WebCam, PointGrey
};

class AScanCamera
{
public:
	AScanCamera();

	/** Camera type�� �Է¹޾� ī�޶� ��ü�� �����մϴ�. */
	AScanCamera(ECameraType CameraType);

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

private:
	ECameraType ObjectCameraType; // ���� Object ���� �� ���õ� Camera Type�� ������ ����

	FlyCapture2::Error FError;
	FlyCapture2::Camera FCamera;
	FlyCapture2::CameraInfo FCameraInfo;
	FlyCapture2::Image RawImage;
	Image RGBImage;
};