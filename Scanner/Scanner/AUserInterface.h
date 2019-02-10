#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/qbuttongroup.h>
#include "ui_AUserInterface.h"

#include <opencv2/aruco/charuco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;
using namespace std;

#include <iostream>
using namespace std;

#define SCAN_RUN 0
#define SCAN_EXPORT 1

class AUserInterface : public QWidget
{
	Q_OBJECT

public:
	AUserInterface(QWidget *parent = Q_NULLPTR);
	Ui::AUserInterfaceClass UserInterfaceAccessor;

	int VisibleBoxInfo;
	int UpdateBoxInfo;
	int ScanBoxInfo;

public slots:
	void VisibleBoxSlot(int Index, bool State);
	void UpdateBoxSlot(int Index, bool State);
	void ScanBoxSlot(int Index, bool State);

	void SetHLine(Mat HLineParams);
	void SetVLine(Mat VLineParams);
	void SetLaserPlane(Mat LaserPlaneParams);
	
private:
	QButtonGroup* VisibleButtonGroup;
	QButtonGroup* UpdateButtonGroup;
	QButtonGroup* ScanButtonGroup;
};
