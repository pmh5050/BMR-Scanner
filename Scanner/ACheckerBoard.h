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

	/** �����ڷκ��� ������ Parameter��� ArUco ���� �� flag ���� �ʱ�ȭ�մϴ� */
	void InitACheckerBoard();
	/** Filename�� �Է¹޾� DetectorParameters�� �����մϴ�*/
	void ReadDetectorParameters(string Filename);

	/** Accessor function */

	/** Detector parameter File�� �о�鿴���� ������ ��ȯ�մϴ� */
	bool IsReadDetectorParameters();
	/** Mat �������� B2C Transform Matrix�� �޾Ƽ� �����մϴ� */
	void SetTransformB2C(Mat NewTrasnformB2C);
	/** Mat �������� B2C Transform Matrix�� ��ȯ�մϴ� */
	Mat GetTransformB2C();
	/** Mat �������� Camera Coordinate�� Normal Vector�� �޾Ƽ� �����մϴ� */
	void SetNormalVector(Mat NewNormalVector);
	/** Mat �������� Camera Coordinate�� ������ �޾Ƽ� �����մϴ� */
	void SetPivotPoint(Mat NewPivotPoint);
	/** Mat �������� Board�� Normal Vector�� ��ȯ�մϴ� */
	Mat GetNormalVector();
	/** Mat �������� Board�� ������ ��ȯ�մϴ� */
	Mat GetPivotPoint();
	/** Mat �������� CheckerBoard�� Plane Parameter�� ���޹޾Ƽ� �����մϴ� */
	void SetPlaneParams(Mat NewPlaneParams);
	/** Mat �������� CheckerBoard�� Plane Parameter�� ��ȯ�մϴ� */
	Mat GetPlaneParams();
	/** CheckerBoard�� ������ Line direction Vector�� Mat�������� �޾� �����մϴ�. */
	void SetLaserLineVector(Mat NewLineVector);
	/** ����� Line direction Vector�� Mat�������� ��ȯ�մϴ�. */
	Mat GetLaserLineVector();
	/** Laser Line Plane�� CheckerBoard�� ���� �� �� ���� ��ġ�� �Է¹޾� Laser Pivot Point�� �����մϴ�. */
	void SetLaserPivotPoint(Mat NewLaserPivotPoint);
	/** Laser Line Plane�� CheckerBoard�� ���� �� �� ���� ��ġ�� ��ȯ�մϴ� */
	Mat GetLaserPivotPoint();
	/** Center Point�� ��ǥ ��ġ(x, y)�� ��ȯ�մϴ�. */
	void GetCenterPoint(double CenterPoint[2]);

	int SquaresX; // CheckerBoard�� ���� ����
	int SquaresY; // CheckerBoard�� ���� ����
	float SquareLength; // CheckerBoard �簢�� �� ���� ����(m)
	float MarkerLength; // CheckerBoard marker �� ���� ����(m)
	int DictionaryId; // ArUco marker Id

	// Detect Parameters
	Ptr<aruco::DetectorParameters> DetectorParams;
	Ptr<aruco::Dictionary> Dictionary;
	Ptr<aruco::CharucoBoard> Charucoboard;
	Ptr<aruco::Board> Board;
	float AxisLength;

private:
	bool bIsReadDetectorParameters; // Detector parameter File�� �о�鿴���� ������ ����
	bool bIsTransformReady; // Transform matrix�� �����ִ��� ������ ����
	Mat TransformB2C; // CheckerBoard Coordinate�� Camera Coordinate�� ��ȯ�ϴ� Transform matrix
	Mat NormalVector; // Camera Coordinate ���� Normal Vector (0, 0, 1, 0)�� �̿��Ͽ� ���
	Mat PivotPoint; // Camera Coordinate ���� ������ ��ǥ (0, 0, 0, 1)�� �̿��Ͽ� ���
	Mat PlaneParams; // Camera Coordinate ���� ��� ������ Paramters
	Mat LaserLineVector; // CheckerBoard�� ������ Line equation�� Parameter �������� ���� (y = ax + b)���� (a, 1, b)
	Mat LaserPivotPoint; // Laser Line Vector�� �����Ǿ��� ��, ��� ���� �����ϴ� ������ �� ��

};

