#include "ALineLaser.h"

ALineLaser::ALineLaser()
{
	MinimumRedValueThreshold = 0;
}


ALineLaser::~ALineLaser()
{
}

/** Mat�������� ���ο� Plane Parameter�� �Է¹޾� �����մϴ�. 
@ Camera Coordinate �������� ���� Params�� �����մϴ�.
*/
void ALineLaser::SetPlaneParams(Mat NewPlaneParams)
{
	PlaneParams = NewPlaneParams.clone();
}

/** Mat�������� ������ Plane Parameter�� ��ȯ�մϴ�. */
Mat ALineLaser::GetPlaneParams()
{
	return PlaneParams;
}

/** �ּҰ� �Ǵ� Red Threshold�� ���� ���������� �޾Ƽ� �����մϴ�. */
void ALineLaser::SetMinimumRedValueThreshold(int NewMinimumRedValueThreshold)
{
	MinimumRedValueThreshold = NewMinimumRedValueThreshold;
}

/** ���������� ����� Red Threshold�� ���� ��ȯ�մϴ�. */
int ALineLaser::GetMinimumRedValueThreshold()
{
	return MinimumRedValueThreshold;
}