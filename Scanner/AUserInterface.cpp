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
	
	// Visible Button Group ����
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
	
	// Update Button Group ����
	{
		UpdateButtonGroup = new QButtonGroup();

		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateHBoardBox, 0);
		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateVBoardBox, 1);
		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateLineLaserBox, 2);
		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateNoneBox, 3);

		QObject::connect(UpdateButtonGroup, SIGNAL(buttonToggled(int, bool)), SLOT(UpdateBoxSlot(int, bool)));
	}

	// Scan Button Group ����
	{
		ScanButtonGroup = new QButtonGroup();
		ScanButtonGroup->setExclusive(false);

		ScanButtonGroup->addButton(UserInterfaceAccessor.ScanBox, 0);
		ScanButtonGroup->addButton(UserInterfaceAccessor.ExportBox, 1);

		QObject::connect(ScanButtonGroup, SIGNAL(buttonToggled(int, bool)), SLOT(ScanBoxSlot(int, bool)));
	}

	// Serial Port Group ����
	{
		bIsSerialOpened = false;

		SerialPort = new QSerialPort();
		QObject::connect(UserInterfaceAccessor.ConnectButton, SIGNAL(released()), SLOT(SerialConnect()));
		QObject::connect(UserInterfaceAccessor.UnitStepButton, SIGNAL(released()), SLOT(StepOnce()));
	}

	// Timer ����
	{
		TimerHandler = new QTimer(this);
		QObject::connect(TimerHandler, SIGNAL(timeout()), SLOT(ToggleTableFlag()));
	}
}

/** Visible Box Widget�� Ȱ��ȭ ������ ����� ��� ȣ��Ǹ�, Ȱ��ȭ ���� ������ �����մϴ�. */
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

/** Update Box Widget�� Ȱ��ȭ ������ ����� ��� ȣ��Ǹ�, Ȱ��ȭ ���� ������ �����մϴ�. */
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

/** Scan Box Widget�� Ȱ��ȭ ������ ����� ��� ȣ��Ǹ�, Ȱ��ȭ ���� ������ �����մϴ�. */
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

/** H Board�� Line Laser�� ������ Mat �������� �޾� �ش� Widget�� ǥ���մϴ�. */
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

/** V Board�� Line Laser�� ������ Mat �������� �޾� �ش� Widget�� ǥ���մϴ�. */
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

/** Line Laser�� �������� Mat �������� �޾� �ش� Widget�� ǥ���մϴ�. */
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

/** Serial Port�� ������ Serial Channel�� Connect�մϴ�. */
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

/** ���� Serial Port�� ���ȴ��� ������ Ȯ���մϴ�. */
bool AUserInterface::IsSerialOpened()
{
	return bIsSerialOpened;
}

/** Timer�� Period(ms)���� StepOnce �Լ��� �����ϰԲ� �����մϴ�. */
void AUserInterface::TimerInit(int PeriodMs)
{
	if (bIsTimerRunning == false)
	{
		TimerHandler->start(PeriodMs);
		bIsTimerRunning = true;
	}
}

/** Timer�� �����մϴ�. */
void AUserInterface::TimerStop()
{
	if (bIsTimerRunning == true)
	{
		TimerHandler->stop();
		bIsTimerRunning = false;
	}
}

/** Serial Port�� 1ȸ Step ��ȣ�� �����մϴ�. */
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
		cout << "Serial Port�� ������� �ʾҽ��ϴ�." << endl;
#endif
	}
}

/** ���� Timer�� �����ϰ� �ִ� �� ������ ��ȯ�մϴ�. */
bool AUserInterface::IsTimerRunning()
{
	return bIsTimerRunning;
}

/** Table Flag�� �����մϴ�. */
void AUserInterface::ToggleTableFlag()
{
	bToggleTableFlag = !bToggleTableFlag;
	cout << bToggleTableFlag << endl;
}

AUserInterface::~AUserInterface()
{
	delete SerialPort;
}
