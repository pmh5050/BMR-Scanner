#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/qbuttongroup.h>
#include "ui_AUserInterface.h"

#include <opencv2/aruco/charuco.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;
using namespace std;

#define VISIBLE_DEFAULT 0
#define VISIBLE_H_BOARD 1
#define VISIBLE_V_BOARD 2
#define VISIBLE_LINE_LASER 3
#define VISIBLE_CURSOR_POSITION 4

#define UPDATE_H_BOARD 0
#define UPDATE_V_BOARD 1
#define UPDATE_LINE_LASER 2
#define UPDATE_NONE 3

#define SCAN_RUN 0
#define SCAN_EXPORT 1

class AUserInterface : public QWidget
{
	Q_OBJECT

public:
	AUserInterface(QWidget *parent = Q_NULLPTR);
	Ui::AUserInterfaceClass UserInterfaceAccessor;

	int VisibleBoxInfo; // Visible Box의 활성화 정보를 저장할 변수
	int UpdateBoxInfo; // Update Box의 활성화 정보를 저장할 변수
	int ScanBoxInfo; // Scan Box의 활성화 정보를 저장할 변수

public slots:
	/** Visible Box Widget의 활성화 정보가 변경될 경우 호출되며, 활성화 정보 변수를 수정합니다. */
	void VisibleBoxSlot(int Index, bool State);
	/** Update Box Widget의 활성화 정보가 변경될 경우 호출되며, 활성화 정보 변수를 수정합니다. */
	void UpdateBoxSlot(int Index, bool State);
	/** Scan Box Widget의 활성화 정보가 변경될 경우 호출되며, 활성화 정보 변수를 수정합니다. */
	void ScanBoxSlot(int Index, bool State);
	/** H Board와 Line Laser의 교선을 Mat 형식으로 받아 해당 Widget에 표시합니다. */
	void SetHLine(Mat HLineParams);
	/** V Board와 Line Laser의 교선을 Mat 형식으로 받아 해당 Widget에 표시합니다. */
	void SetVLine(Mat VLineParams);
	/** Line Laser의 방정식을 Mat 형식으로 받아 해당 Widget에 표시합니다. */
	void SetLaserPlane(Mat LaserPlaneParams);
	
private:
	QButtonGroup* VisibleButtonGroup; // Visible Button들을 관리할 Button Group Pointer
	QButtonGroup* UpdateButtonGroup; // Update Button들을 관리할 Button Group Pointer
	QButtonGroup* ScanButtonGroup; // Scan Button들을 관리할 Button Group Pointer
};
