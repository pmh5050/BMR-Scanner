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
	class ALinkedList* LinkedListHead; // Item 포함

	/** 검출의 대상이 되는 CheckerBoard를 설정합니다. 
	@ 'H' : H Checkerboard가 검출의 대상이 됩니다.
	@ 'V' : V Checkerboard가 검출의 대상이 됩니다.
	*/
	void SetObjectCheckerBoard(char NewObjectCheckerBoardType);
	/** 검출의 대상이 되는 ChekcerBoard의 Pointer를 반환합니다. */
	class ACheckerBoard* GetObjectCheckerBoard();
	/** ObjectCheckerBoard를 검출한 뒤, member class에 해당 정보를 저장합니다. */
	void DetectCheckerBoard(Mat& Frame);
	/** ObjectCheckerBoard의 Rotation matrix 및 Translate matrix를 이용하여 Transform maxtrix를 Update 합니다. */
	void UpdateCheckerBoardTransformMatrix();
	/** Frame을 입력받아 검출한 CheckerBoard의 축을 표시합니다. */
	void DrawCheckerBoardAxis(Mat& Frame);
	/** CheckerBoard의 Normal Vector와 Pivot Point를 Camera Coordinate에 대해서 Update 합니다. */
	void UpdateCheckerBoardParams();

	/** 선택된 CheckerBoard의 종류에 따라 Line Update를 수행합니다 */
	void UpdateCheckerBoardLaserLine(Mat& Frame);

	/** 현재 Frame이 Pose가 존재하는지 여부를 반환합니다 */
	bool IsValidPose();
	/** WindowHandler를 이용하여 CurrentCursorData를 Update 합니다 */
	void UpdateCursorData();
	POINT CurrentCursorData; // Cursor point accesor
 
	/** Checkerboard들의 Line Params를 이용하여, Camera coordinate 기준 Line laser의 Plane Params를 Update 합니다. */
	void UpdateLineLaserPlaneParams();
	
	/** Scan data가 존재하는지 여부를 반환합니다. */
	bool IsScanDataReady();

	/** Scan data로부터 Point Cloud 정보를 계산한 뒤 해당 FileName에 저장합니다 */
	void StorePointCloud(string FileName);

	/** Scan data로부터 Point Cloud 정보를 계산한 뒤 해당 FileName에 저장합니다, 추가로 ScanVolume을 입력받아서 해당 범위를 벗어난 경우 Point에서 열외합니다.
	@ Scan Volume의 형태에서 첫 번째 index는 Axis에 해당하며, 두 번째 index의 경우 0이면 minimum, 1이면 maximum에 해당합니다.
	@ (예시) : Scan Volume[0][0]의 경우 X Axis의 minimum 값에 해당합니다.
	*/
	void StorePointCloud(string FileName, double ScanVolume[3][2]);

	/** Scan을 진행합니다. */
	void ScanRunning();

private:
	class ACheckerBoard* ObjectCheckerBoard;

	bool bIsValidPose;
	bool bIsHLineReady;
	bool bIsVLineReady;
	bool bIsScanDataReady;

	// ArUco marker 검출 결과를 저장할 변수들
	vector<int> MarkerIds, CharucoIds;
	vector<vector<Point2f>> MarkerCorners, RejectedMarkers;
	vector<Point2f> CharucoCorners;
	Vec3d vRot, vTrs;

	
	HWND WindowHandler;
	
};

