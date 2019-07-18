#pragma once
#include <windows.h>
#include <iostream>

#include <QtWidgets/QApplication>


#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;

enum EOffsetType : int;

class AScanner
{
public:
	AScanner(int argc, char **argv);

	/** Frame�� �Է¹޾� ������ CheckerBoard�� ���� ǥ���մϴ�. */
	void DrawCheckerBoardAxis(Mat& Frame);
	/** Frame�� �Է¹޾� ������ CheckerBoard�� ���� Offset Option�� ����Ͽ� ǥ���մϴ�. */
	void DrawCheckerBoardAxis(Mat& Frame, EOffsetType OffsetType);
	/** Scan data�κ��� Point Cloud ������ ����� �� �ش� FileName�� �����մϴ� */
	void StorePointCloud(string FileName);
	/**
	* Scan data�κ��� Point Cloud ������ ����� �� �ش� FileName�� �����մϴ�, �߰��� ScanVolume�� �Է¹޾Ƽ� �ش� ������ ��� ��� Point���� �����մϴ�.
	* @param Scan Volume - ù ��° index�� Axis�� �ش��ϸ�, �� ��° index�� ��� 0�̸� minimum, 1�̸� maximum�� �ش��մϴ�.
	*/
	void StorePointCloud(string FileName, double ScanVolume[3][2]);
	/**
	* Scan data�κ��� Point Cloud ������ ����� �� �ش� FileName�� �����մϴ�, �߰��� ScanVolume�� �Է¹޾Ƽ� �ش� ������ ��� ��� Point���� �����մϴ�.
	* @param Scan Volume - ù ��° index�� Axis�� �ش��ϸ�, �� ��° index�� ��� 0�̸� minimum, 1�̸� maximum�� �ش��մϴ�.
	* @param WindowSize - Anti-Aliasing�� ���� Windows�� ũ�⿡ �ش��մϴ�.
	*/
	void StorePointCloud(string FileName, double ScanVolume[3][2], int WindowSize);
	/**
	* Scan data�κ��� Point Cloud ������ ����� �� �ش� FileName�� �����մϴ�, �߰��� ScanVolume�� �Է¹޾Ƽ� �ش� ������ ��� ��� Point���� �����մϴ�.
	* @param Scan Volume - ù ��° index�� Axis�� �ش��ϸ�, �� ��° index�� ��� 0�̸� minimum, 1�̸� maximum�� �ش��մϴ�.
	* @param WindowSize - Anti-Aliasing�� ���� Windows�� ũ�⿡ �ش��մϴ�.
	*/
	void StorePointCloudWithoutKF(string FileName, double ScanVolume[3][2], int WindowSize);

	/** Scan�� 1ȸ �����մϴ�. (Ÿ�̸Ӹ� �������� �ʽ��ϴ�.) */
	void ScanRunningUntilTimer();
	/** Scan�� �����մϴ�. */
	void ScanRunning();

	/** Accessor funtion */

	/** 
	* ������ ����� �Ǵ� CheckerBoard�� �����մϴ�. 
	* @param NewObjectCheckerBoardType - 'H'�� ��� H Checkerboard�� ������ ����� �Ǹ�, 'V'�� ��� V Checkerboard�� ������ ����� �˴ϴ�.
	*/
	void SetObjectCheckerBoard(char NewObjectCheckerBoardType);
	/** ������ ����� �Ǵ� ChekcerBoard�� Pointer�� ��ȯ�մϴ�. */
	class ACheckerBoard* GetObjectCheckerBoard();
	/** ObjectCheckerBoard�� ������ ��, member class�� �ش� ������ �����մϴ�. */
	void DetectCheckerBoard(Mat& Frame);
	/** ObjectCheckerBoard�� Rotation matrix �� Translate matrix�� �̿��Ͽ� Transform maxtrix�� Update �մϴ�. */
	void UpdateCheckerBoardTransformMatrix();
	/** CheckerBoard�� Normal Vector�� Pivot Point�� Camera Coordinate�� ���ؼ� Update �մϴ�. */
	void UpdateCheckerBoardParams();
	/** ���õ� CheckerBoard�� ������ ���� Line Update�� �����մϴ� */
	void UpdateCheckerBoardLaserLine(Mat& Frame);
	/** ���� Frame�� Pose�� �����ϴ��� ���θ� ��ȯ�մϴ� */
	bool IsValidPose();
	/** WindowHandler�� �̿��Ͽ� CurrentCursorData�� Update �մϴ� */
	void UpdateCursorData();
	/** Checkerboard���� Line Params�� �̿��Ͽ�, Camera coordinate ���� Line laser�� Plane Params�� Update �մϴ�. */
	void UpdateLineLaserPlaneParams();
	/** Scan data�� �����ϴ��� ���θ� ��ȯ�մϴ�. */
	bool IsScanDataReady();
	/** ACheckerBoard Class�� ������ ������� Checkerboard�� �߽���ǥ ���� Update �մϴ�. */
	void UpdateCenterPoint();
	/** Pseudo Inverse�� ����� ���ڷ� �޾Ƽ� ���� Checkerboard�� ȸ�� �߽���ǥ ���� Update �մϴ�. */
	void UpdateOptimalPoint(Mat NewOptimalPoint);
	/** AScanner�� Linked List�� ���Ե� Data�� �ʱ�ȭ�մϴ�. */
	void ClearScanData();
	/** ���� Object Checkerboard�� Yaw angle�� ��ȯ�մϴ�. */
	double GetYawAngle();

	QApplication* Application;
	class AUserInterface* UserInterface;

	class AScanCamera* ScanCamera;
	class ACheckerBoard* HCheckerBoard;
	class ACheckerBoard* VCheckerBoard;
	class ALineLaser* LineLaser;
	class ALinkedList* LinkedListHead; // Item ����

	POINT CurrentCursorData; // Cursor point accesor

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

	Mat CenterPoint; // Checkerboard�� �߽� ��ǥ
	Mat OptimalPoint; // Checkerboard�� ȸ�� �߽� ��ǥ
	
	HWND WindowHandler;
};

