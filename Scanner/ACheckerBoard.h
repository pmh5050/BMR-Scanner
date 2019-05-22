#pragma once

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;
using namespace std;

class ACheckerBoard
{
public:
	ACheckerBoard(int NewSquaresX, int NewSquaresY, float NewSquareLength, float NewMarkerLength, int NewDictionaryId);

	/** 생성자로부터 결정된 Parameter들로 ArUco 변수 및 flag 등을 초기화합니다 */
	void InitACheckerBoard();
	/** Filename을 입력받아 DetectorParameters를 설정합니다*/
	void ReadDetectorParameters(string Filename);

	/** Accessor function */

	/** Detector parameter File을 읽어들였는지 유무를 반환합니다 */
	bool IsReadDetectorParameters();
	/** Mat 형식으로 B2C Transform Matrix를 받아서 저장합니다 */
	void SetTransformB2C(Mat NewTrasnformB2C);
	/** Mat 형식으로 B2C Transform Matrix를 반환합니다 */
	Mat GetTransformB2C();
	/** Mat 형식으로 Camera Coordinate의 Normal Vector를 받아서 저장합니다 */
	void SetNormalVector(Mat NewNormalVector);
	/** Mat 형식으로 Camera Coordinate의 원점을 받아서 저장합니다 */
	void SetPivotPoint(Mat NewPivotPoint);
	/** Mat 형식으로 Board의 Normal Vector를 반환합니다 */
	Mat GetNormalVector();
	/** Mat 형식으로 Board의 원점을 반환합니다 */
	Mat GetPivotPoint();
	/** Mat 형식으로 CheckerBoard의 Plane Parameter를 전달받아서 저장합니다 */
	void SetPlaneParams(Mat NewPlaneParams);
	/** Mat 형식으로 CheckerBoard의 Plane Parameter를 반환합니다 */
	Mat GetPlaneParams();
	/** CheckerBoard에 투영된 Line direction Vector를 Mat형식으로 받아 저장합니다. */
	void SetLaserLineVector(Mat NewLineVector);
	/** 저장된 Line direction Vector를 Mat형식으로 반환합니다. */
	Mat GetLaserLineVector();
	/** Laser Line Plane과 CheckerBoard의 교점 중 한 점의 위치를 입력받아 Laser Pivot Point에 저장합니다. */
	void SetLaserPivotPoint(Mat NewLaserPivotPoint);
	/** Laser Line Plane과 CheckerBoard의 교점 중 한 점의 위치를 반환합니다 */
	Mat GetLaserPivotPoint();
	/** Center Point의 좌표 위치(x, y)를 반환합니다. */
	void GetCenterPoint(double CenterPoint[2]);

	int SquaresX; // CheckerBoard의 가로 개수
	int SquaresY; // CheckerBoard의 세로 개수
	float SquareLength; // CheckerBoard 사각형 한 변의 길이(m)
	float MarkerLength; // CheckerBoard marker 한 변의 길이(m)
	int DictionaryId; // ArUco marker Id

	// Detect Parameters
	Ptr<aruco::DetectorParameters> DetectorParams;
	Ptr<aruco::Dictionary> Dictionary;
	Ptr<aruco::CharucoBoard> Charucoboard;
	Ptr<aruco::Board> Board;
	float AxisLength;

private:
	bool bIsReadDetectorParameters; // Detector parameter File을 읽어들였는지 유무를 저장
	bool bIsTransformReady; // Transform matrix를 갖고있는지 유무를 저장
	Mat TransformB2C; // CheckerBoard Coordinate를 Camera Coordinate로 변환하는 Transform matrix
	Mat NormalVector; // Camera Coordinate 기준 Normal Vector (0, 0, 1, 0)을 이용하여 계산
	Mat PivotPoint; // Camera Coordinate 기준 원점의 좌표 (0, 0, 0, 1)을 이용하여 계산
	Mat PlaneParams; // Camera Coordinate 기준 평면 방정식 Paramters
	Mat LaserLineVector; // CheckerBoard에 투영된 Line equation을 Parameter 형식으로 저장 (y = ax + b)에서 (a, 1, b)
	Mat LaserPivotPoint; // Laser Line Vector가 결정되었을 때, 평면 위에 존재하는 임의의 한 점

};

