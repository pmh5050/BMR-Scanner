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
	// Mat�� ��� �ڵ����� �ݳ���

	// ���� Node�� �ݳ���
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

/** ���� Node�� Image ������ ���� �ִ��� ������ ��ȯ�մϴ� */
bool AScanDataSet::IsReadyImgData()
{
	return bIsReadyImgData;
}

/** Mat �������� B2C Transform Matrix�� �޾Ƽ� �����մϴ� */
void AScanDataSet::SetTransformB2C(Mat NewTrasnformB2C)
{
	TransformB2C = NewTrasnformB2C.clone();
}

/** Mat �������� B2C Transform Matrix�� ��ȯ�մϴ� */
Mat AScanDataSet::GetTransformB2C()
{
	return TransformB2C;
}