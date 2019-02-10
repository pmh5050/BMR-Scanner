#include "AScanCamera.h"

#define ASCANCAMERA_DEBUG true

AScanCamera::AScanCamera()
{
	Capture.open(CAMERA_INDEX);
	if (!Capture.isOpened())
	{
		cout << "ERROR : Camera가 존재하지 않습니다." << endl;
	}
}

AScanCamera::~AScanCamera()
{

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

bool AScanCamera::IsFrameReady()
{
	return Capture.grab();
}

bool AScanCamera::GetFrame(Mat& Frame)
{
	Mat Temp;
	bool IsGetFrame = Capture.read(Temp);
	Frame = Temp.clone();

	return IsGetFrame;
}

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