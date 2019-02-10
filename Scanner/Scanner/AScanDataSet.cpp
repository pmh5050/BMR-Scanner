#include "AScanDataSet.h"

#include <iostream>
#include <opencv2/highgui.hpp>

using namespace std;

AScanDataSet::AScanDataSet()
{
	DeltaAngle = 0.0f;
	DeltaTimeMs = 0;
	bIsReadyImgData = false;
}

void AScanDataSet::DestroyNode()
{
#if ASCANDATASET_DEBUG true
	cout << "Destroy the AScanDataSet Node : " << this << endl;
#endif
	// Mat의 경우 자동으로 반납됨

	// 현재 Node를 반납함
	delete this;
}

AScanDataSet::~AScanDataSet()
{

}

float AScanDataSet::GetDeltaAngle()
{
	return DeltaAngle;
}

int AScanDataSet::GetDeltaTimeMs()
{
	return DeltaTimeMs;
}

Mat AScanDataSet::GetOrgImageData()
{
	return OrgImageData;
}

void AScanDataSet::SetDeltaAngle(float NewDeltaAngle)
{
	DeltaAngle = NewDeltaAngle;
}

void AScanDataSet::SetDeltaTimeMs(int NewDeltaTimeMs)
{
	DeltaTimeMs = NewDeltaTimeMs;
}

void AScanDataSet::SetOrgImageData(Mat NewImageData)
{
	OrgImageData = NewImageData.clone();
	bIsReadyImgData = true;
}

/** 현재 Node가 Image 정보를 갖고 있는지 유무를 반환합니다 */
bool AScanDataSet::IsReadyImgData()
{
	return bIsReadyImgData;
}

/** Mat 형식으로 B2C Transform Matrix를 받아서 저장합니다 */
void AScanDataSet::SetTransformB2C(Mat NewTrasnformB2C)
{
	TransformB2C = NewTrasnformB2C.clone();
}

/** Mat 형식으로 B2C Transform Matrix를 반환합니다 */
Mat AScanDataSet::GetTransformB2C()
{
	return TransformB2C;
}