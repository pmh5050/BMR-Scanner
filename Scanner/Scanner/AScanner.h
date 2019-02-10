#pragma once
#include <windows.h>

#include <QtWidgets/QApplication>
#include "AUserInterface.h"

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>

using namespace std;
using namespace cv;

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define RANGE_MINIMUM 0
#define RANGE_MAXIMUM 1

class AScanner
{
public:
	AScanner(int argc, char **argv);
	~AScanner();

	QApplication* Application;
	AUserInterface* UserInterface;
	
	class AScanCamera* ScanCamera;
	class ACheckerBoard* HCheckerBoard;
	class ACheckerBoard* VCheckerBoard;
	class ALineLaser* LineLaser;
	class AScannerHelper* ScannerHelper;
	class ALinkedList* LinkedListHead; // Item ����

	/** ������ ����� �Ǵ� CheckerBoard�� �����մϴ�. 
	@ 'H' : H Checkerboard�� ������ ����� �˴ϴ�.
	@ 'V' : V Checkerboard�� ������ ����� �˴ϴ�.
	*/
	void SetObjectCheckerBoard(char NewObjectCheckerBoardType);
	/** ������ ����� �Ǵ� ChekcerBoard�� Pointer�� ��ȯ�մϴ�. */
	class ACheckerBoard* GetObjectCheckerBoard();
	/** ObjectCheckerBoard�� ������ ��, member class�� �ش� ������ �����մϴ�. */
	void DetectCheckerBoard(Mat& Frame);
	/** ObjectCheckerBoard�� Rotation matrix �� Translate matrix�� �̿��Ͽ� Transform maxtrix�� Update �մϴ�. */
	void UpdateCheckerBoardTransformMatrix();
	/** Frame�� �Է¹޾� ������ CheckerBoard�� ���� ǥ���մϴ�. */
	void DrawCheckerBoardAxis(Mat& Frame);
	/** CheckerBoard�� Normal Vector�� Pivot Point�� Camera Coordinate�� ���ؼ� Update �մϴ�. */
	void UpdateCheckerBoardParams();

	/** ���õ� CheckerBoard�� ������ ���� Line Update�� �����մϴ� */
	void UpdateCheckerBoardLaserLine(Mat& Frame);

	/** ���� Frame�� Pose�� �����ϴ��� ���θ� ��ȯ�մϴ� */
	bool IsValidPose();
	/** WindowHandler�� �̿��Ͽ� CurrentCursorData�� Update �մϴ� */
	void UpdateCursorData();
	POINT CurrentCursorData; // Cursor point accesor
 
	/** Checkerboard���� Line Params�� �̿��Ͽ�, Camera coordinate ���� Line laser�� Plane Params�� Update �մϴ�. */
	void UpdateLineLaserPlaneParams();
	
	/** Scan data�� �����ϴ��� ���θ� ��ȯ�մϴ�. */
	bool IsScanDataReady();

	/** Scan data�κ��� Point Cloud ������ ����� �� �ش� FileName�� �����մϴ� */
	void StorePointCloud(string FileName);

	/** Scan data�κ��� Point Cloud ������ ����� �� �ش� FileName�� �����մϴ�, �߰��� ScanVolume�� �Է¹޾Ƽ� �ش� ������ ��� ��� Point���� �����մϴ�.
	@ Scan Volume�� ���¿��� ù ��° index�� Axis�� �ش��ϸ�, �� ��° index�� ��� 0�̸� minimum, 1�̸� maximum�� �ش��մϴ�.
	@ (����) : Scan Volume[0][0]�� ��� X Axis�� minimum ���� �ش��մϴ�.
	*/
	void StorePointCloud(string FileName, double ScanVolume[3][2]);

	/** Scan�� �����մϴ�. */
	void ScanRunning();

private:
	class ACheckerBoard* ObjectCheckerBoard;

	bool bIsValidPose;
	bool bIsHLineReady;
	bool bIsVLineReady;
	bool bIsScanDataReady;

	// ArUco marker ���� ����� ������ ������
	vector<int> MarkerIds, CharucoIds;
	vector<vector<Point2f>> MarkerCorners, RejectedMarkers;
	vector<Point2f> CharucoCorners;
	Vec3d vRot, vTrs;

	
	HWND WindowHandler;
	
};

