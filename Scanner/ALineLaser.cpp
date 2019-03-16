#include "ALineLaser.h"

ALineLaser::ALineLaser()
{
	MinimumRedValueThreshold = 4;
}

/** Mat�������� ���ο� Plane Parameter(Camera coordinate)�� �Է¹޾� �����մϴ�. */
void ALineLaser::SetPlaneParams(Mat NewPlaneParams)
{
	PlaneParams = NewPlaneParams.clone();
}

/** �ּҰ� �Ǵ� Red Threshold�� ���� ���������� �޾Ƽ� �����մϴ�. */
void ALineLaser::SetMinimumRedValueThreshold(int NewMinimumRedValueThreshold)
{
	MinimumRedValueThreshold = NewMinimumRedValueThreshold;
}

/** Mat�������� ������ Plane Parameter�� ��ȯ�մϴ�. */
Mat ALineLaser::GetPlaneParams()
{
	return PlaneParams;
}

/** ���������� ����� Red Threshold�� ���� ��ȯ�մϴ�. */
int ALineLaser::GetMinimumRedValueThreshold()
{
	return MinimumRedValueThreshold;
}