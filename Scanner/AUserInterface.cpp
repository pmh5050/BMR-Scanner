#include "AUserInterface.h"

#define AUSERINTERFACE_DEBUG true

#define CAMERA_COORDINATE 0
#define BOARD_COORDINATE 1
#define INFO_COORDIANTE BOARD_COORDINATE

AUserInterface::AUserInterface(QWidget *parent)
	: QWidget(parent)
{
	UserInterfaceAccessor.setupUi(this);

	VisibleBoxInfo = 0x00;
	UpdateBoxInfo = 0x00;
	ScanBoxInfo = 0x00;
	
	// Visible Button Group 구성
	{
		VisibleButtonGroup = new QButtonGroup();

		UserInterfaceAccessor.VisibleDefault->setChecked(true);
		VisibleBoxInfo = 1;
		VisibleButtonGroup->addButton(UserInterfaceAccessor.VisibleDefault, 0);
		VisibleButtonGroup->addButton(UserInterfaceAccessor.VisibleHBoard, 1);
		VisibleButtonGroup->addButton(UserInterfaceAccessor.VisibleVBoard, 2);
		VisibleButtonGroup->addButton(UserInterfaceAccessor.VisibleLineLaser, 3);
		VisibleButtonGroup->addButton(UserInterfaceAccessor.VisibleCursorPosition, 4);

		QObject::connect(VisibleButtonGroup, SIGNAL(buttonToggled(int, bool)), SLOT(VisibleBoxSlot(int, bool)));
	}
	
	// Update Button Group 구성
	{
		UpdateButtonGroup = new QButtonGroup();

		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateHBoardBox, 0);
		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateVBoardBox, 1);
		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateLineLaserBox, 2);
		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateNoneBox, 3);

		QObject::connect(UpdateButtonGroup, SIGNAL(buttonToggled(int, bool)), SLOT(UpdateBoxSlot(int, bool)));
	}

	// Scan Button Group 구성
	{
		ScanButtonGroup = new QButtonGroup();
		ScanButtonGroup->setExclusive(false);

		ScanButtonGroup->addButton(UserInterfaceAccessor.ScanBox, 0);
		ScanButtonGroup->addButton(UserInterfaceAccessor.ExportBox, 1);

		QObject::connect(ScanButtonGroup, SIGNAL(buttonToggled(int, bool)), SLOT(ScanBoxSlot(int, bool)));
	}

	// Serial Port Group 구성
	{
		bIsSerialOpened = false;

		SerialPort = new QSerialPort();
		QObject::connect(UserInterfaceAccessor.ConnectButton, SIGNAL(released()), SLOT(SerialConnect()));
		QObject::connect(UserInterfaceAccessor.UnitStepButton, SIGNAL(released()), SLOT(StepOnce()));
	}

	// Timer 구성
	{
		TimerHandler = new QTimer(this);
		QObject::connect(TimerHandler, SIGNAL(timeout()), SLOT(ToggleTableFlag()));
	}
}

/** Visible Box Widget의 활성화 정보가 변경될 경우 호출되며, 활성화 정보 변수를 수정합니다. */
void AUserInterface::VisibleBoxSlot(int Index, bool State)
{
	int Mask = 0x00;
	Mask |= (1 << Index);
	Mask ^= 0xFF;
	VisibleBoxInfo = (VisibleBoxInfo & Mask) | (State << Index);
#if AUSERINTERFACE_DEBUG
	cout << VisibleBoxInfo << endl;
#endif
}

/** Update Box Widget의 활성화 정보가 변경될 경우 호출되며, 활성화 정보 변수를 수정합니다. */
void AUserInterface::UpdateBoxSlot(int Index, bool State)
{
	int Mask = 0x00;
	Mask |= (1 << Index);
	Mask ^= 0xFF;
	UpdateBoxInfo = (UpdateBoxInfo & Mask) | (State << Index);
#if AUSERINTERFACE_DEBUG
	cout << UpdateBoxInfo << endl;
#endif
}

/** Scan Box Widget의 활성화 정보가 변경될 경우 호출되며, 활성화 정보 변수를 수정합니다. */
void AUserInterface::ScanBoxSlot(int Index, bool State)
{
	int Mask = 0x00;
	Mask |= (1 << Index);
	Mask ^= 0xFF;
	ScanBoxInfo = (ScanBoxInfo & Mask) | (State << Index);
#if AUSERINTERFACE_DEBUG
	cout << ScanBoxInfo << endl;
#endif
}

/** H Board와 Line Laser의 교선을 Mat 형식으로 받아 해당 Widget에 표시합니다. */
void AUserInterface::SetHLine(Mat HLineParams)
{
	QString HLineInfoString = "H Board\n";

	double* MatrixPointer = HLineParams.ptr<double>(0);
	for (int i = 0; i < 3; i++)
	{
#if (INFO_COORDIANTE == BOARD_COORDINATE)
		double Value = MatrixPointer[i];
		switch (i)
		{
		case 0:
			HLineInfoString.append("X gain : ");
			break;
		case 1:
			HLineInfoString.append(", Y gain : ");
			break;
		case 2:
			HLineInfoString.append(", Constant : ");
			break;
		}
		
		if (Value > 0)
		{
			HLineInfoString.append("+");
		}
		QString ValueString = QString::number(Value, 'f', 2);
		HLineInfoString.append(ValueString);
#endif
	}
	UserInterfaceAccessor.InfoHBoard->setText(HLineInfoString);
}

/** V Board와 Line Laser의 교선을 Mat 형식으로 받아 해당 Widget에 표시합니다. */
void AUserInterface::SetVLine(Mat VLineParams)
{
	QString VLineInfoString = "V Board\n";

	double* MatrixPointer = VLineParams.ptr<double>(0);
	for (int i = 0; i < 3; i++)
	{
#if (INFO_COORDIANTE == BOARD_COORDINATE)
		double Value = MatrixPointer[i];
		switch (i)
		{
		case 0:
			VLineInfoString.append("X gain : ");
			break;
		case 1:
			VLineInfoString.append(", Y gain : ");
			break;
		case 2:
			VLineInfoString.append(", Constant : ");
			break;
		}

		if (Value > 0)
		{
			VLineInfoString.append("+");
		}
		QString ValueString = QString::number(Value, 'f', 2);
		VLineInfoString.append(ValueString);
#endif
	}
	UserInterfaceAccessor.InfoVBoard->setText(VLineInfoString);
}

/** Line Laser의 방정식을 Mat 형식으로 받아 해당 Widget에 표시합니다. */
void AUserInterface::SetLaserPlane(Mat LaserPlaneParams)
{
	QString LaserPlaneString = "Line Laser\n";

	double* MatrixPointer = LaserPlaneParams.ptr<double>(0);

	for (int i = 0; i < 4; i++)
	{
		double Value = MatrixPointer[i];
		if (Value > 0)
		{
			LaserPlaneString.append("+");
		}
		QString ValueString = QString::number(Value, 'f', 2);
		LaserPlaneString.append(ValueString);
		switch (i)
		{
		case 0:
			LaserPlaneString.append("x");
			break;
		case 1:
			LaserPlaneString.append("y");
			break;
		case 2:
			LaserPlaneString.append("z");
			break;
		}
	}
	LaserPlaneString.append("=0");
	UserInterfaceAccessor.InfoLineLaser->setText(LaserPlaneString);
}

/** Serial Port를 선택한 Serial Channel에 Connect합니다. */
void AUserInterface::SerialConnect()
{
	QString SerialPortString = QString("COM") + UserInterfaceAccessor.SerialChannel->text();
	SerialPort->setPortName(SerialPortString);
	SerialPort->setBaudRate(QSerialPort::Baud9600);
	SerialPort->setDataBits(QSerialPort::Data8);
	SerialPort->setParity(QSerialPort::NoParity);
	SerialPort->setStopBits(QSerialPort::OneStop);
	SerialPort->setFlowControl(QSerialPort::NoFlowControl);

	if (!SerialPort->open(QIODevice::ReadWrite))
	{
		bIsSerialOpened = false;
	}
	else
	{
		bIsSerialOpened = true;
		UserInterfaceAccessor.SerialStateLabel->setText(SerialPortString + " was connected");
	}
}

/** 현재 Serial Port가 열렸는지 유무를 확인합니다. */
bool AUserInterface::IsSerialOpened()
{
	return bIsSerialOpened;
}

/** Timer가 Period(ms)마다 StepOnce 함수를 수행하게끔 설정합니다. */
void AUserInterface::TimerInit(int PeriodMs)
{
	if (bIsTimerRunning == false)
	{
		TimerHandler->start(PeriodMs);
		bIsTimerRunning = true;
	}
}

/** Timer를 중지합니다. */
void AUserInterface::TimerStop()
{
	if (bIsTimerRunning == true)
	{
		TimerHandler->stop();
		bIsTimerRunning = false;
	}
}

/** Serial Port에 1회 Step 신호를 전송합니다. */
void AUserInterface::StepOnce()
{
	if (IsSerialOpened())
	{
		char SendData[] = "<X001!";
		SendData[1] = 10;
		SerialPort->write(SendData);
	}
	else
	{
#if AUSERINTERFACE_DEBUG == true
		cout << "Serial Port가 연결되지 않았습니다." << endl;
#endif
	}
}

/** 현재 Timer가 동작하고 있는 지 유무를 반환합니다. */
bool AUserInterface::IsTimerRunning()
{
	return bIsTimerRunning;
}

/** Table Flag를 반전합니다. */
void AUserInterface::ToggleTableFlag()
{
	bToggleTableFlag = !bToggleTableFlag;
	cout << bToggleTableFlag << endl;
}

AUserInterface::~AUserInterface()
{
	delete SerialPort;
}
