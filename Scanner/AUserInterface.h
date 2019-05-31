#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/qbuttongroup.h>
#include <QtSerialPort/qserialport.h>
#include <QTimer>

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
	~AUserInterface();
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
	/** Serial Port를 선택한 Serial Channel에 Connect합니다. */
	void SerialConnect();
	/** 현재 Serial Port가 열렸는지 유무를 확인합니다. */
	bool IsSerialOpened();

	/** Timer가 Period(ms)마다 StepOnce 함수를 수행하게끔 설정합니다. */
	void TimerInit(int PeriodMs);

	/** Timer를 중지합니다. */
	void TimerStop();

	/** Serial Port에 1회 Step 신호를 전송합니다. */
	void StepOnce();

	/** 현재 Timer가 동작하고 있는 지 유무를 반환합니다. */
	bool IsTimerRunning();

	/** Table Flag를 반전합니다. */
	void ToggleTableFlag();

	
private:
	QButtonGroup* VisibleButtonGroup; // Visible Button들을 관리할 Button Group Pointer
	QButtonGroup* UpdateButtonGroup; // Update Button들을 관리할 Button Group Pointer
	QButtonGroup* ScanButtonGroup; // Scan Button들을 관리할 Button Group Pointer

	QTimer* TimerHandler; // 타이머 Handler
	QSerialPort* SerialPort; // Arduino Uno와 통신할 Serial Port Handler

	bool bIsSerialOpened; // 현재 Serial Port가 열렸는지 유무를 저장할 변수

	bool bIsTimerRunning; // 현재 Timer가 동작하고 있는지 유무를 저장할 변수	
public:

	bool bToggleTableFlag;
};
