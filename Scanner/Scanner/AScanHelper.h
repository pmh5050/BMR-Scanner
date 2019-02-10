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
	/** ������ ���� �ش� ������ ����� ���ϵ��� Clipping �մϴ�. */
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

	/** �� Vector�� �Է¹޾� ������ �մϴ�. */
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

	/** PlaneParams�� RayVector�� �Է¹��� ��, ������ ����Ͽ� ��ȯ�մϴ�.
	@ PlaneParams�� ��� Line Laser Plane�� Camera Coordinate�� ǥ���� Parameter�鿡 �ش��մϴ�.
	@ RayVector�� ��� Ray�� ���� ���� ���͸� Camera Coordinate�� ǥ���� Parameter�鿡 �ش��մϴ�.
	@ ��ȯ ���� ��� ���������� Camera Coordinate�� ǥ���� ������ �ش��ϸ� ���� ��ȯ�� �̿��Ͽ� ������ ���˴ϴ�.
	*/
	Mat CalIntersectionPoint(Mat PlaneParams, Mat RayVector)
	{
		Mat IntersectionPoint(1, 3, CV_64F);
		
		// ���

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
				// Red point Ž�� ����
				RedPointPosition[i] = -1;
			}
			else
			{
				RedPointPosition[i] = MaxRedIndex;
			}
		}
	}

	/** 'Transform matrix'�� 'CheckerBoard's PlaneParams' �� 'Camera ��ü'��Red Point�� Position���� �Է¹޾� �� Board�� ������ Line�� CheckerBoard Coordinate�� ��ȯ�մϴ�. */
	/*
	void CalLinearRegression(Mat TransformMatrix, Mat PlaneParams, AScanCamera* ScanCamera, int RedPointPosition[])
	{
		//double CameraX = ScanCamera->mCamMatrix
		for (int i = 0; i < HEIGHT_SIZE; i++)
		{
			if (RedPointPosition[i] != -1) // �ش� Row�� Red Point�� �ִ� ���
			{
				double dRayVector[3] = { i, RedPointPosition[i], 1 };
				//Mat RayVector(1, 3, CV_64F, dRayVector);
				
			}
		}
		//return RayVector;
	}
	*/
	/** Point�� Transform Matrix�� �̿��Ͽ�, ��ǥ�� ��ȯ�� �������ݴϴ�. */
	Mat PointCoordinateTransform(Mat Point, Mat TransformMatrix)
	{
		Mat ConvertedPoint(1, 4, CV_64F);
		
		// ���

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

	/** Frame�� ���ڷ� �־ Line laser�� ǥ�õ� Frame�� ��ȯ�մϴ�. */
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
