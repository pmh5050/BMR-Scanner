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

/** 자신의 Node를 소멸시킴 */
void AScanDataSet::DestroyNode()
{
#if ASCANDATASET_DEBUG
	cout << "Destroy the AScanDataSet Node : " << this << endl;
#endif
	// Mat의 경우 자동으로 반납됨

	// 현재 Node를 반납함
	delete this;
}

/** 현재 Node의 Delta Angle 값을 반환합니다 */
float AScanDataSet::GetDeltaAngle()
{
	return DeltaAngle;
}

/** 현재 Node의 Delta Time 값을 ms 단위로 반환합니다 */
int AScanDataSet::GetDeltaTimeMs()
{
	return DeltaTimeMs;
}

/** 현재 Node의 원본 Image 값을 Mat 형태로 반환합니다 */
Mat AScanDataSet::GetOrgImageData()
{
	return OrgImageData;
}

/** 현재 Node의 Delta Angle 값을 저장합니다 */
void AScanDataSet::SetDeltaAngle(float NewDeltaAngle)
{
	DeltaAngle = NewDeltaAngle;
}

/** 현재 Node의 Delta Time 값을 ms 단위로 저장합니다 */
void AScanDataSet::SetDeltaTimeMs(int NewDeltaTimeMs)
{
	DeltaTimeMs = NewDeltaTimeMs;
}

/** 현재 Node의 원본 Image 값을 Mat 형태로 저장합니다 */
void AScanDataSet::SetOrgImageData(Mat NewImageData)
{
	OrgImageData = NewImageData.clone();
	bIsReadyImgData = true;
}

/** 현재 Node의 B2C Transform Matrix를 Mat 형태로 저장합니다 */
void AScanDataSet::SetTransformB2C(Mat NewTrasnformB2C)
{
	TransformB2C = NewTrasnformB2C.clone();
}

/** 현재 Node의 B2C Transform Matrix를 Mat 형태로 반환합니다 */
Mat AScanDataSet::GetTransformB2C()
{
	return TransformB2C.clone();
}

/** 현재 Node가 Image 정보를 갖고 있는지 유무를 반환합니다 */
bool AScanDataSet::IsReadyImgData()
{
	return bIsReadyImgData;
}

/** 현재 Node의 Image에서 인식된 Marker의 Count를 정수 형태로 저장합니다 */
void AScanDataSet::SetDetectedMarkerCount(int NewDetectedMarkerCount)
{
	DetectedMarkerCount = NewDetectedMarkerCount;

}

/** 현재 Node의 Image에서 인식된 Marker의 Count를 정수 형태로 반환합니다 */
int AScanDataSet::GetDetectedMarkerCount()
{
	return DetectedMarkerCount;

}

/** 현재 Node에서 Step motor가 Step한 횟수를 정수 형태로 저장합니다 */
void AScanDataSet::SetDeltaStepCount(int NewDeltaStepCount)
{
	DeltaStepCount = NewDeltaStepCount;
}

/** 현재 Node에서 Step motor가 Step한 횟수를 정수 형태로 반환합니다 */
int AScanDataSet::GetDeltaStepCount()
{
	return DeltaStepCount;
}

/** 현재 Node에서 Frame이 Checkerboard에 대한 Pose를 정상적으로 검출했는 지 여부를 논리 형태로 저장합니다 */
void AScanDataSet::SetIsValidPose(bool NewIsValidPose)
{
	bIsValidPose = NewIsValidPose;
}

/** 현재 Node에서 Frame이 Checkerboard에 대한 Pose를 정상적으로 검출했는 지 여부를 논리 형태로 반환합니다 */
bool AScanDataSet::GetIsValidPose()
{
	return bIsValidPose;
}

/** 현재 시점에서 Scan이 시작된 이후로 경과된 시간을 정수형태로 저장합니다. */
void AScanDataSet::SetElapsedTime(int NewElapsedTime)
{
	ElapsedTime = NewElapsedTime;
}

/** 현재 시점에서 Scan이 시작된 이후로 경과된 시간을 정수형태로 반환합니다. */
int AScanDataSet::GetElpasedTime()
{
	return ElapsedTime;
}