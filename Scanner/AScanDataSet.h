#pragma once

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;

class AScanDataSet
{
public:
	AScanDataSet();

	/** 자신의 Node를 소멸시킴 */
	void DestroyNode();

	/** Accessor funtion */

	/** 현재 Node의 Delta Angle 값을 반환합니다 */
	float GetDeltaAngle();
	/** 현재 Node의 Delta Time 값을 ms 단위로 반환합니다 */
	int GetDeltaTimeMs();
	/** 현재 Node의 원본 Image 값을 Mat 형태로 반환합니다 */
	Mat GetOrgImageData();
	/** 현재 Node의 Delta Angle 값을 저장합니다 */
	void SetDeltaAngle(float NewDeltaAngle);
	/** 현재 Node의 Delta Time 값을 ms 단위로 저장합니다 */
	void SetDeltaTimeMs(int NewDeltaTimeMs);
	/** 현재 Node의 원본 Image 값을 Mat 형태로 저장합니다 */
	void SetOrgImageData(Mat NewImageData);
	/** 현재 Node의 B2C Transform Matrix를 Mat 형태로 저장합니다 */
	void SetTransformB2C(Mat NewTrasnformB2C);
	/** 현재 Node의 B2C Transform Matrix를 Mat 형태로 반환합니다 */
	Mat GetTransformB2C();
	/** 현재 Node가 Image 정보를 갖고 있는지 유무를 반환합니다 */
	bool IsReadyImgData();
	/** 현재 Node의 Image에서 인식된 Marker의 Count를 정수 형태로 저장합니다 */
	void SetDetectedMarkerCount(int NewDetectedMarkerCount);
	/** 현재 Node의 Image에서 인식된 Marker의 Count를 정수 형태로 반환합니다 */
	int GetDetectedMarkerCount();

	/** 현재 Node에서 Step motor가 Step한 횟수의 변화량을 정수 형태로 저장합니다 */
	void SetDeltaStepCount(int NewDeltaStepCount);
	/** 현재 Node에서 Step motor가 Step한 횟수의 변화량을 정수 형태로 반환합니다 */
	int GetDeltaStepCount();

	/** 현재 Node에서 Frame이 Checkerboard에 대한 Pose를 정상적으로 검출했는 지 여부를 논리 형태로 저장합니다 */
	void SetIsValidPose(bool NewIsValidPose);
	/** 현재 Node에서 Frame이 Checkerboard에 대한 Pose를 정상적으로 검출했는 지 여부를 논리 형태로 반환합니다 */
	bool GetIsValidPose();

private:
	float DeltaAngle; // 현재 frame의 회전각을 저장할 변수
	int DeltaTimeMs; // 현재 frame의 경과시간을 저장할 변수
	Mat OrgImageData; // 현재 frame의 Image 정보를 저장할 변수
	bool bIsReadyImgData; // 현재 Node가 Image 정보를 갖고 있는지 유무를 저장할 변수
	Mat TransformB2C; // 현재 Node의 Frame이 갖고있는 B2C Transform matrix
	int DetectedMarkerCount; // 현재 Node의 Image에서 인식된 Marker의 Count를 저장할 변수
	int DeltaStepCount; // 현재 Node에서 Step motor가 Step한 횟수의 변화량을 저장할 변수
	bool bIsValidPose; // 현재 Node에서 Frame이 Checkerboard에 대한 Pose를 정상적으로 검출했는 지 여부를 저장할 변수
};