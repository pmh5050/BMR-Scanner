#pragma once

#include <iostream>
#include <math.h>

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

//#include <AScanCamera.h>

using namespace std;
using namespace cv;

#define WIDTH_INDEX 0
#define HEIGHT_INDEX 1

#define WIDTH_SIZE 640
#define HEIGHT_SIZE 480

namespace ScanHelper
{
	/** 변수의 값이 해당 범위를 벗어나지 못하도록 Clipping 합니다. */
	int Clipping(int Value, int Min, int Max)
	{
		if (Value < Min)
		{
			Value = Min;
		}
		else if (Value > Max)
		{
			Value = Max;
		}
		return Value;
	}

	/** 두 Vector를 입력받아 외적을 합니다. */
	Mat CalCrossProduct(Mat Vector1, Mat Vector2)
	{
		Mat CrossProductVector(3, 1, CV_64F);
		int Pointer[2];
		for (int i = 0; i < 3; i++)
		{
			Pointer[0] = (i + 1) % 3;
			Pointer[1] = (i + 2) % 3;
			CrossProductVector.at<double>(i) = Vector1.at<double>(Pointer[0]) * Vector2.at<double>(Pointer[1]) - Vector1.at<double>(Pointer[1]) * Vector2.at<double>(Pointer[0]);
		}
		return CrossProductVector;
	}

	/** PlaneParams와 RayVector를 입력받은 뒤, 교점을 계산하여 반환합니다.
	@ PlaneParams의 경우 Line Laser Plane을 Camera Coordinate로 표현한 Parameter들에 해당합니다.
	@ RayVector의 경우 Ray에 대한 방향 벡터를 Camera Coordinate로 표현한 Parameter들에 해당합니다.
	@ 반환 값의 경우 마찬가지로 Camera Coordinate로 표현한 교점에 해당하며 이후 변환을 이용하여 복원에 사용됩니다.
	*/
	Mat CalIntersectionPoint(Mat PlaneParams, Mat RayVector)
	{
		Mat IntersectionPoint(1, 3, CV_64F);
		
		// 계산

		return IntersectionPoint;
	}

	void CalGaussianFrame(Mat Frame, int ImageSize[2], int FilterLength, double StandardDerivation)
	{
		Mat GaussianFrame(ImageSize[WIDTH_INDEX], ImageSize[HEIGHT_INDEX], CV_64F);
		int* GaussianFilter = new int[FilterLength];
		double FilterSum = 0.0f;
		// Make Gaussian filter (Kernal)
		for (int i = 0; i < FilterLength; i++)
		{
			double N = -(i * i);
			double D = 2 * StandardDerivation * StandardDerivation;
			GaussianFilter[i] = exp(N / D);
			FilterSum += GaussianFilter[i];
		}

		FilterSum = (FilterSum - GaussianFilter[0]) * 2 + GaussianFilter[0];

		// Calculation Gaussian filter
		for (int j = 0; j < ImageSize[HEIGHT_INDEX]; j++)
		{
			uchar* pointer_input = Frame.ptr<uchar>(j);
			uchar* pointer_ouput = GaussianFrame.ptr<uchar>(j);
			for (int i = 0; i < ImageSize[WIDTH_INDEX]; i++)
			{
				double GaussianValue[3] = { 0, 0, 0 };
				for (int k = 0; k < FilterLength; k++)
				{
					GaussianValue[0] = pointer_input[i * 3 + 0]; // Blue
					GaussianValue[1] = pointer_input[i * 3 + 1]; // Green
					GaussianValue[2] = pointer_input[i * 3 + 2]; // Red
				}
					
				pointer_ouput[i] = (GaussianValue[0]) / 3.0;
			}
		}

		delete GaussianFilter;
	}

	void CalcRedPoint(Mat Frame, int MinRedThreshold, int RedPointPosition[])
	{	
		for (int i = 0; i < HEIGHT_SIZE; i++)
		{
			int MaxRedValue = -255;
			int MaxRedIndex = 0;
			
			for (int j = 0; j < WIDTH_SIZE; j++)
			{
				uchar* FramePointer = Frame.ptr<uchar>(i);
				uchar B = FramePointer[j * 3 + 0];
				uchar G = FramePointer[j * 3 + 1];
				uchar R = FramePointer[j * 3 + 2];
				int RedValue = R - (G + B) / 2;
				if (RedValue > MaxRedValue)
				{
					MaxRedValue = RedValue;
					MaxRedIndex = j;
				}
			}

			if (MaxRedValue < MinRedThreshold)
			{
				// Red point 탐색 실패
				RedPointPosition[i] = -1;
			}
			else
			{
				RedPointPosition[i] = MaxRedIndex;
			}
		}
	}

	/** 'Transform matrix'와 'CheckerBoard's PlaneParams' 및 'Camera 객체'와Red Point의 Position들을 입력받아 각 Board에 투영된 Line을 CheckerBoard Coordinate로 반환합니다. */
	/*
	void CalLinearRegression(Mat TransformMatrix, Mat PlaneParams, AScanCamera* ScanCamera, int RedPointPosition[])
	{
		//double CameraX = ScanCamera->mCamMatrix
		for (int i = 0; i < HEIGHT_SIZE; i++)
		{
			if (RedPointPosition[i] != -1) // 해당 Row에 Red Point가 있는 경우
			{
				double dRayVector[3] = { i, RedPointPosition[i], 1 };
				//Mat RayVector(1, 3, CV_64F, dRayVector);
				
			}
		}
		//return RayVector;
	}
	*/
	/** Point와 Transform Matrix를 이용하여, 좌표계 변환을 수행해줍니다. */
	Mat PointCoordinateTransform(Mat Point, Mat TransformMatrix)
	{
		Mat ConvertedPoint(1, 4, CV_64F);
		
		// 계산

		return ConvertedPoint;
	}

	Mat ConvertGray()
	{
		Mat Gray(HEIGHT_SIZE, WIDTH_SIZE, CV_8U);
		for (int i = 0; i < HEIGHT_SIZE; i++)
		{
			uchar* FramePointer = Gray.ptr<uchar>(i);
			for (int j = 0; j < WIDTH_SIZE; j++)
			{
				FramePointer[j] = (i + j)/5;
			}
		}
		return Gray;
	}

	/** Frame을 인자로 주어서 Line laser만 표시된 Frame을 반환합니다. */
	Mat GetLineLaserFrame(Mat Frame)
	{
		Mat LineLaserFrame(HEIGHT_SIZE, WIDTH_SIZE, CV_8U);
		int PositionArray[HEIGHT_SIZE];
		CalcRedPoint(Frame, 50, PositionArray);

		for (int i = 0; i < HEIGHT_SIZE; i++)
		{
			uchar* FramePointer = LineLaserFrame.ptr<uchar>(i);
			for (int j = 0; j < WIDTH_SIZE; j++)
			{
				if (PositionArray[i] == j)
				{
					FramePointer[j] = 255;
				}
				else
				{
					FramePointer[j] = 0;
				}
			}
		}
		return LineLaserFrame;
	}

}
