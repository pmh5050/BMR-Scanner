#include "ALineLaser.h"

ALineLaser::ALineLaser()
{
	MinimumRedValueThreshold = 0;
}


ALineLaser::~ALineLaser()
{
}

/** Mat형식으로 새로운 Plane Parameter를 입력받아 저장합니다. 
@ Camera Coordinate 기준으로 계산된 Params를 저장합니다.
*/
void ALineLaser::SetPlaneParams(Mat NewPlaneParams)
{
	PlaneParams = NewPlaneParams.clone();
}

/** Mat형식으로 저장한 Plane Parameter를 반환합니다. */
Mat ALineLaser::GetPlaneParams()
{
	return PlaneParams;
}

/** 최소가 되는 Red Threshold의 값을 정수형으로 받아서 저장합니다. */
void ALineLaser::SetMinimumRedValueThreshold(int NewMinimumRedValueThreshold)
{
	MinimumRedValueThreshold = NewMinimumRedValueThreshold;
}

/** 정수형으로 저장된 Red Threshold의 값을 반환합니다. */
int ALineLaser::GetMinimumRedValueThreshold()
{
	return MinimumRedValueThreshold;
}