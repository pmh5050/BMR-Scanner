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

};

