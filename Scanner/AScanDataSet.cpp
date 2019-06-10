#include "AScanDataSet.h"

#include <iostream>

using namespace std;

#define ASCANDATASET_DEBUG false

AScanDataSet::AScanDataSet()
{
	DeltaAngle = 0.0f;
	DeltaTimeMs = 0;
	bIsReadyImgData = false;
}

/** �ڽ��� Node�� �Ҹ��Ŵ */
void AScanDataSet::DestroyNode()
{
#if ASCANDATASET_DEBUG
	cout << "Destroy the AScanDataSet Node : " << this << endl;
#endif
	// Mat�� ��� �ڵ����� �ݳ���

	// ���� Node�� �ݳ���
	delete this;
}

/** ���� Node�� Delta Angle ���� ��ȯ�մϴ� */
float AScanDataSet::GetDeltaAngle()
{
	return DeltaAngle;
}

/** ���� Node�� Delta Time ���� ms ������ ��ȯ�մϴ� */
int AScanDataSet::GetDeltaTimeMs()
{
	return DeltaTimeMs;
}

/** ���� Node�� ���� Image ���� Mat ���·� ��ȯ�մϴ� */
Mat AScanDataSet::GetOrgImageData()
{
	return OrgImageData;
}

/** ���� Node�� Delta Angle ���� �����մϴ� */
void AScanDataSet::SetDeltaAngle(float NewDeltaAngle)
{
	DeltaAngle = NewDeltaAngle;
}

/** ���� Node�� Delta Time ���� ms ������ �����մϴ� */
void AScanDataSet::SetDeltaTimeMs(int NewDeltaTimeMs)
{
	DeltaTimeMs = NewDeltaTimeMs;
}

/** ���� Node�� ���� Image ���� Mat ���·� �����մϴ� */
void AScanDataSet::SetOrgImageData(Mat NewImageData)
{
	OrgImageData = NewImageData.clone();
	bIsReadyImgData = true;
}

/** ���� Node�� B2C Transform Matrix�� Mat ���·� �����մϴ� */
void AScanDataSet::SetTransformB2C(Mat NewTrasnformB2C)
{
	TransformB2C = NewTrasnformB2C.clone();
}

/** ���� Node�� B2C Transform Matrix�� Mat ���·� ��ȯ�մϴ� */
Mat AScanDataSet::GetTransformB2C()
{
	return TransformB2C.clone();
}

/** ���� Node�� Image ������ ���� �ִ��� ������ ��ȯ�մϴ� */
bool AScanDataSet::IsReadyImgData()
{
	return bIsReadyImgData;
}

/** ���� Node�� Image���� �νĵ� Marker�� Count�� ���� ���·� �����մϴ� */
void AScanDataSet::SetDetectedMarkerCount(int NewDetectedMarkerCount)
{
	DetectedMarkerCount = NewDetectedMarkerCount;

}

/** ���� Node�� Image���� �νĵ� Marker�� Count�� ���� ���·� ��ȯ�մϴ� */
int AScanDataSet::GetDetectedMarkerCount()
{
	return DetectedMarkerCount;

}

/** ���� Node���� Step motor�� Step�� Ƚ���� ���� ���·� �����մϴ� */
void AScanDataSet::SetStepCount(int NewStepCount)
{
	StepCount = NewStepCount;
}

/** ���� Node���� Step motor�� Step�� Ƚ���� ���� ���·� ��ȯ�մϴ� */
int AScanDataSet::GetStepCount()
{
	return StepCount;
}