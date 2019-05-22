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

	/** Camera type을 입력받아 카메라 객체를 생성합니다. */
	AScanCamera(ECameraType CameraType);

	~AScanCamera();

	/** Filename을 입력받아 카메라의 내부 파라미터 및 왜곡 계수를 설정합니다 */
	void ReadCameraParameters(string Filename);

	bool IsFrameReady();
	bool GetFrame(Mat& Frame);
	void ShowFrame();
	
	VideoCapture Capture;

	// Camera Parameters
	Mat mCamMatrix; // 카메라 내부 파라미터
	Mat mDistCoeffs; // 카메라 외곡 계수

	bool bIsReadCameraParameters; // Camera parameter File을 읽어들였는지 유무를 저장

	double CenterOffset[2];
	double FocalLength[2];
	// SVD

private:
	ECameraType ObjectCameraType; // 현재 Object 생성 시 선택된 Camera Type을 저장할 변수

	FlyCapture2::Error FError;
	FlyCapture2::Camera FCamera;
	FlyCapture2::CameraInfo FCameraInfo;
	FlyCapture2::Image RawImage;
	Image RGBImage;
};