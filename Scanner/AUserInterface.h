#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/qbuttongroup.h>
#include <QtSerialPort/qserialport.h>
#include <QTimer>
#include <QFileDialog>

#include "ui_AUserInterface.h"

#include <opencv2/aruco/charuco.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include "AScanner.h"

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

enum ELabelType 
{
	RGBFrame,
	LaserFrame
};

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

	int RedTh; // RedTh 값을 조정하는데 참조될 변수
	int WindowSize; // Anti Aliasing에 참조되는 Window Size를 저장할 변수
	double LimitXY; // Scan Volume에 참조되는 수평 방향 최대 길이
	double LimitZ; // Scan Volume에 참조되는 수직 방향 최대 길이
	double OdometryCC; // Odometry model의 분산 특성에 참조되는 상수 [0:1]
	double MeasurementCC; // Measurement model의 분산 특성에 참조되는 상수 [0:1]

	/** AScanner의 포인터를 받아 참조 변수에 등록합니다. */
	void SetScannerReference(AScanner* ScannerReference);

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

	/** 현재 Plane Parameter를 Save 합니다. */
	void SaveScannerCalibrationData();

	/** Camera Calibration File의 Path를 Load합니다. */
	void LoadCameraCalibrationPath();

	/** Load한 Camera Calibration Path를 기반으로 Update합니다. */
	void UpdateCameraCalibrationFromPath();

	/** Scanner Calibration File의 Path를 Load합니다. */
	void LoadScannerCalibrationPath();

	/** Load한 Scanner Calibration Path를 기반으로 Update합니다. */
	void UpdateScannerCalibrationFromPath();

	/** Label Type과 Frame 정보를 입력받아 해당 Label을 Update합니다. */
	void UpdateLabelFromFrame(ELabelType LabelType, Mat& Frame);

	/** Spin Value 조정 값을 기반으로 Redth 값을 Update 합니다. */
	void UpdateRedThreshold(int SpinValue);
	/** Spin Value 조정 값을 기반으로 Anti Aliasing에 참조되는 Window Size 값을 Update 합니다. */
	void UpdateWindowSize(int SpinValue);
	/** Slider의 조정 값을 기반으로 수평 방향 Scan Volume의 제한치를 Update 합니다. */
	void UpdateLimitXY(int SlideValue);
	/** Slider의 조정 값을 기반으로 수직 방향 Scan Volume의 제한치를 Update 합니다. */
	void UpdateLimitZ(int SlideValue);
	/** Slider의 조정 값을 기반으로 Odometry model의 분산에 관여하는 특성 상수 값을 Update 합니다. */
	void UpdateOdometryCharacteristicConstant(int SlideValue);
	/** Slider의 조정 값을 기반으로 Measurement model의 분산에 관여하는 특성 상수 값을 Update 합니다. */
	void UpdateMeasurementCharacteristicConstant(int SlideValue);

	/** Reference의 Scan Data를 초기화합니다. */
	void ClearReferenceScanData();
	/** Reference의 Scan Data를 Processing하여 Draw합니다. */
	void ProcessingFromReferenceScanData();
	
private:
	AScanner* Scanner; // AScanner의 주소를 보관할 Reference 변수
	QButtonGroup* VisibleButtonGroup; // Visible Button들을 관리할 Button Group Pointer
	QButtonGroup* UpdateButtonGroup; // Update Button들을 관리할 Button Group Pointer
	QButtonGroup* ScanButtonGroup; // Scan Button들을 관리할 Button Group Pointer

	QTimer* TimerHandler; // 타이머 Handler
	QSerialPort* SerialPort; // Arduino Uno와 통신할 Serial Port Handler

	QString CameraCalibrationPath; // Camera Calibration Path를 저장할 변수
	QString ScannerCalibrationPath; // Scanner Calibration Path를 저장할 변수

	bool bIsSerialOpened; // 현재 Serial Port가 열렸는지 유무를 저장할 변수

	bool bIsTimerRunning; // 현재 Timer가 동작하고 있는지 유무를 저장할 변수	
public:

	bool bToggleTableFlag;
};
