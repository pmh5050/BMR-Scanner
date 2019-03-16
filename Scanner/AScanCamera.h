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

	/** Filename을 입력받아 카메라의 내부 파라미터 및 왜곡 계수를 설정합니다 */
	void ReadCameraParameters(string Filename);

	/** Accessor funtion */

	/** 현재 Camera의 Frame의 준비 여부를 반환합니다 */
	bool IsFrameReady();
	/** 현재 Camera의 Frame을 획득한 뒤, 인자로 받은 객체의 주소에 반환합니다 */
	bool GetFrame(Mat& Frame);
	/** 현재 Camera의 Frame을 획득 및 출력합니다 */
	void ShowFrame();
	
	// Camera Parameters
	Mat mCamMatrix; // 카메라 내부 파라미터
	Mat mDistCoeffs; // 카메라 외곡 계수
	double CenterOffset[2];
	double FocalLength[2];

private:
	VideoCapture Capture; // Camera module accessor 객체
	bool bIsReadCameraParameters; // Camera parameter File을 읽어들였는지 유무를 저장

};

