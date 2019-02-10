#pragma once

#define ASCANDATASET_DEBUG false

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;

class AScanDataSet
{
public:
	AScanDataSet();
	~AScanDataSet();

	/** 자신의 Node를 소멸시킴 */
	void DestroyNode();

	/** Accessor 함수 */
	float GetDeltaAngle();
	int GetDeltaTimeMs();
	Mat GetOrgImageData();

	void SetDeltaAngle(float NewDeltaAngle);
	void SetDeltaTimeMs(int NewDeltaTimeMs);
	void SetOrgImageData(Mat NewImageData);
	/** Mat 형식으로 B2C Transform Matrix를 받아서 저장합니다 */
	void SetTransformB2C(Mat NewTrasnformB2C);
	/** Mat 형식으로 B2C Transform Matrix를 반환합니다 */
	Mat GetTransformB2C();
	
	/** 현재 Node가 Image 정보를 갖고 있는지 유무를 반환합니다 */
	bool IsReadyImgData();

private:
	/** 현재 frame의 회전각을 저장할 변수 */
	float DeltaAngle;
	/** 현재 frame의 경과시간을 저장할 변수 */
	int DeltaTimeMs;
	/** 현재 frame의 Image 정보를 저장할 변수 */
	Mat OrgImageData;
	/** 현재 Node가 Image 정보를 갖고 있는지 유무를 저장할 변수 */
	bool bIsReadyImgData;
	/** 현재 Node의 Frame이 갖고있는 B2C Transform matrix */
	Mat TransformB2C; // CheckerBoard Coordinate를 Camera Coordinate로 변환하는 Transform matrix
};