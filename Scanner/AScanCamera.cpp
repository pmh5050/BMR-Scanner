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

/** Camera type을 입력받아 카메라 객체를 생성합니다. */
AScanCamera::AScanCamera(ECameraType CameraType)
{
	switch (CameraType)
	{
	case WebCam:
		Capture.open(CAMERA_INDEX);
		if (!Capture.isOpened())
		{
			cout << "ERROR : Camera가 존재하지 않습니다." << endl;
		}

		ObjectCameraType = CameraType;
		break;
	case PointGrey:
		FError = FCamera.Connect(0);
		if (FError != PGRERROR_OK)
		{
			cout << "Failed to connect to camera" << endl;
		}

		FError = FCamera.GetCameraInfo(&FCameraInfo);
		if (FError != PGRERROR_OK)
		{
			cout << "Failed to get camera info from camera" << endl;
		}
		cout << FCameraInfo.vendorName << " " << FCameraInfo.modelName << " " << FCameraInfo.serialNumber << endl;

		FError = FCamera.StartCapture();
		if (FError == PGRERROR_ISOCH_BANDWIDTH_EXCEEDED)
		{
			std::cout << "Bandwidth exceeded" << std::endl;
		}
		else if (FError != PGRERROR_OK)
		{
			std::cout << "Failed to start image capture" << std::endl;
		}

		ObjectCameraType = CameraType;
		break;
	default:
		cout << "[ERROR] 잘못된 Camera Type 입니다." << endl;
		break;
	}
}

AScanCamera::~AScanCamera()
{
	switch (ObjectCameraType)
	{
	case WebCam:

		break;
	case PointGrey:
		FError = FCamera.StopCapture();
		if (FError != PGRERROR_OK)
		{
			// This may fail when the camera was removed, so don't show 
			// an error message
		}
		FCamera.Disconnect();
		break;
	default:

		break;
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

bool AScanCamera::IsFrameReady()
{
	bool bIsFrameReady;
	switch (ObjectCameraType)
	{
	case WebCam:
		bIsFrameReady = Capture.grab();
		break;
	case PointGrey:
		// Get the image
		FError = FCamera.RetrieveBuffer(&RawImage);
		if (FError != PGRERROR_OK)
		{
			std::cout << "capture error" << std::endl;
			bIsFrameReady = false;
		}
		else
		{
			bIsFrameReady = true;
		}
		break;
	default:
		bIsFrameReady = false;
		break;
	}

	return bIsFrameReady;
}

bool AScanCamera::GetFrame(Mat& Frame)
{
	Mat Temp;

	bool bIsGetFrame = false;
	unsigned int RowBytes;
	switch (ObjectCameraType)
	{
	case WebCam:
		bIsGetFrame = Capture.read(Temp);
		Frame = Temp.clone();
		break;
	case PointGrey:
		bIsGetFrame = true;
		RawImage.Convert(FlyCapture2::PIXEL_FORMAT_BGR, &RGBImage);
		RowBytes = (double)RGBImage.GetReceivedDataSize() / (double)RGBImage.GetRows();
		Frame = cv::Mat(RGBImage.GetRows(), RGBImage.GetCols(), CV_8UC3, RGBImage.GetData(), RowBytes).clone();
		break;
	default:
		bIsGetFrame = false;
		break;
	}

	

	return bIsGetFrame;
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