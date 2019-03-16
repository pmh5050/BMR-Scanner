#include "AScanCamera.h"

#define ASCANCAMERA_DEBUG true

#define CAMERA_INDEX 0

AScanCamera::AScanCamera()
{
	Capture.open(CAMERA_INDEX);
	/*
	Capture.set(CAP_PROP_AUTO_EXPOSURE, 0);
	Capture.set(CAP_PROP_EXPOSURE, -9.5f);
	*/

	if (!Capture.isOpened())
	{
		cout << "ERROR : Camera가 존재하지 않습니다." << endl;
	}
}

/** Filename을 입력받아 카메라의 내부 파라미터 및 왜곡 계수를 설정합니다 */
void AScanCamera::ReadCameraParameters(string Filename)
{
	FileStorage Fs(Filename, FileStorage::READ);
	if (!Fs.isOpened())
	{
#if ASCANCAMERA_DEBUG
		cout << "File을 읽는데 실패하였습니다." << endl;
		cout << "File name : " << Filename << endl;
#endif
		bIsReadCameraParameters = false;
	}
	else
	{
		Fs["camera_matrix"] >> mCamMatrix;
		Fs["distortion_coefficients"] >> mDistCoeffs;

		double* MatrixPointer = mCamMatrix.ptr<double>(0);
		FocalLength[0] = MatrixPointer[0];
		CenterOffset[0] = MatrixPointer[2];
		MatrixPointer = mCamMatrix.ptr<double>(1);
		FocalLength[1] = MatrixPointer[1];
		CenterOffset[1] = MatrixPointer[2];
		
		bIsReadCameraParameters = true;
	}
}

/** 현재 Camera의 Frame의 준비 여부를 반환합니다 */
bool AScanCamera::IsFrameReady()
{
	return Capture.grab();
}

/** 현재 Camera의 Frame을 획득한 뒤, 인자로 받은 객체의 주소에 반환합니다 */
bool AScanCamera::GetFrame(Mat& Frame)
{
	Mat Temp;
	bool IsGetFrame = Capture.read(Temp);
	Frame = Temp.clone();

	return IsGetFrame;
}

/** 현재 Camera의 Frame을 획득 및 출력합니다 */
void AScanCamera::ShowFrame()
{
	Mat Frame;
	if (GetFrame(Frame))
	{
		imshow("Frame", Frame);
	}
	else
	{
		cout << "Frame loss" << endl;
	}
}