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
		cout << "ERROR : Camera�� �������� �ʽ��ϴ�." << endl;
	}
}

/** Filename�� �Է¹޾� ī�޶��� ���� �Ķ���� �� �ְ� ����� �����մϴ� */
void AScanCamera::ReadCameraParameters(string Filename)
{
	FileStorage Fs(Filename, FileStorage::READ);
	if (!Fs.isOpened())
	{
#if ASCANCAMERA_DEBUG
		cout << "File�� �дµ� �����Ͽ����ϴ�." << endl;
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

/** ���� Camera�� Frame�� �غ� ���θ� ��ȯ�մϴ� */
bool AScanCamera::IsFrameReady()
{
	return Capture.grab();
}

/** ���� Camera�� Frame�� ȹ���� ��, ���ڷ� ���� ��ü�� �ּҿ� ��ȯ�մϴ� */
bool AScanCamera::GetFrame(Mat& Frame)
{
	Mat Temp;
	bool IsGetFrame = Capture.read(Temp);
	Frame = Temp.clone();

	return IsGetFrame;
}

/** ���� Camera�� Frame�� ȹ�� �� ����մϴ� */
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