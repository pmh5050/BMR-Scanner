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

	int VisibleBoxInfo; // Visible Box�� Ȱ��ȭ ������ ������ ����
	int UpdateBoxInfo; // Update Box�� Ȱ��ȭ ������ ������ ����
	int ScanBoxInfo; // Scan Box�� Ȱ��ȭ ������ ������ ����

public slots:
	/** Visible Box Widget�� Ȱ��ȭ ������ ����� ��� ȣ��Ǹ�, Ȱ��ȭ ���� ������ �����մϴ�. */
	void VisibleBoxSlot(int Index, bool State);
	/** Update Box Widget�� Ȱ��ȭ ������ ����� ��� ȣ��Ǹ�, Ȱ��ȭ ���� ������ �����մϴ�. */
	void UpdateBoxSlot(int Index, bool State);
	/** Scan Box Widget�� Ȱ��ȭ ������ ����� ��� ȣ��Ǹ�, Ȱ��ȭ ���� ������ �����մϴ�. */
	void ScanBoxSlot(int Index, bool State);
	/** H Board�� Line Laser�� ������ Mat �������� �޾� �ش� Widget�� ǥ���մϴ�. */
	void SetHLine(Mat HLineParams);
	/** V Board�� Line Laser�� ������ Mat �������� �޾� �ش� Widget�� ǥ���մϴ�. */
	void SetVLine(Mat VLineParams);
	/** Line Laser�� �������� Mat �������� �޾� �ش� Widget�� ǥ���մϴ�. */
	void SetLaserPlane(Mat LaserPlaneParams);
	/** Serial Port�� ������ Serial Channel�� Connect�մϴ�. */
	void SerialConnect();
	/** ���� Serial Port�� ���ȴ��� ������ Ȯ���մϴ�. */
	bool IsSerialOpened();

	/** Timer�� Period(ms)���� StepOnce �Լ��� �����ϰԲ� �����մϴ�. */
	void TimerInit(int PeriodMs);

	/** Timer�� �����մϴ�. */
	void TimerStop();

	/** Serial Port�� 1ȸ Step ��ȣ�� �����մϴ�. */
	void StepOnce();

	/** ���� Timer�� �����ϰ� �ִ� �� ������ ��ȯ�մϴ�. */
	bool IsTimerRunning();

	/** Table Flag�� �����մϴ�. */
	void ToggleTableFlag();

	
private:
	QButtonGroup* VisibleButtonGroup; // Visible Button���� ������ Button Group Pointer
	QButtonGroup* UpdateButtonGroup; // Update Button���� ������ Button Group Pointer
	QButtonGroup* ScanButtonGroup; // Scan Button���� ������ Button Group Pointer

	QTimer* TimerHandler; // Ÿ�̸� Handler
	QSerialPort* SerialPort; // Arduino Uno�� ����� Serial Port Handler

	bool bIsSerialOpened; // ���� Serial Port�� ���ȴ��� ������ ������ ����

	bool bIsTimerRunning; // ���� Timer�� �����ϰ� �ִ��� ������ ������ ����	
public:

	bool bToggleTableFlag;
};
