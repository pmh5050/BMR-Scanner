#include "AUserInterface.h"

#include "ALinkedList.h"
#include "AScanCamera.h"
#include "ALineLaser.h"
#include "AScannerHelper.h"

#define AUSERINTERFACE_DEBUG true

#define CAMERA_COORDINATE 0
#define BOARD_COORDINATE 1
#define INFO_COORDIANTE BOARD_COORDINATE

#define MAX_TEXT_SIZE 100

vector<string> SplitByCharacter(string OrgString, char Delemiter) {
	vector<string> SplitedString;
	stringstream StringStream(OrgString);
	string PartitionString;

	while (getline(StringStream, PartitionString, Delemiter)) 
	{
		SplitedString.push_back(PartitionString);
	}

	return SplitedString;
}

AUserInterface::AUserInterface(QWidget *parent)
	: QWidget(parent)
{
	UserInterfaceAccessor.setupUi(this);

	Scanner = nullptr;

	VisibleBoxInfo = 0x00;
	UpdateBoxInfo = 0x00;
	ScanBoxInfo = 0x00;
	
	// Visible Button Group ����
	{
		VisibleButtonGroup = new QButtonGroup();

		UserInterfaceAccessor.VisibleDefault->setChecked(true);
		VisibleBoxInfo = 1;
		
		VisibleButtonGroup->setExclusive(false);
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

	// Information Button ����
	{
		QObject::connect(UserInterfaceAccessor.SCSaveButton, SIGNAL(released()), SLOT(SaveScannerCalibrationData()));
	}

	// Scan Button Group ����
	{
		ScanButtonGroup = new QButtonGroup();
		ScanButtonGroup->setExclusive(false);

		ScanButtonGroup->addButton(UserInterfaceAccessor.ScanBox, 0);
		ScanButtonGroup->addButton(UserInterfaceAccessor.ExportBox, 1);

		QObject::connect(ScanButtonGroup, SIGNAL(buttonToggled(int, bool)), SLOT(ScanBoxSlot(int, bool)));

		QObject::connect(UserInterfaceAccessor.ScanDataClearButton, SIGNAL(released()), SLOT(ClearReferenceScanData()));
		QObject::connect(UserInterfaceAccessor.ScanProcessingButton, SIGNAL(released()), SLOT(ProcessingFromReferenceScanData()));
	}

	// Serial Port Group ����
	{
		bIsSerialOpened = false;
		StepCount = 0;

		SerialPort = new QSerialPort();
		QObject::connect(UserInterfaceAccessor.ConnectButton, SIGNAL(released()), SLOT(SerialConnect()));
		QObject::connect(UserInterfaceAccessor.UnitStepButton, SIGNAL(released()), SLOT(StepOnce()));
	}

	// Timer ����
	{
		TimerHandler = new QTimer(this);
		bIsTimerRunning = false;

		QObject::connect(TimerHandler, SIGNAL(timeout()), SLOT(StepOnce()));

	}

	// Calibration ����
	{
		QObject::connect(UserInterfaceAccessor.CCLoadButton, SIGNAL(released()), SLOT(LoadCameraCalibrationPath()));
		QObject::connect(UserInterfaceAccessor.CCUpdateButton, SIGNAL(released()), SLOT(UpdateCameraCalibrationFromPath()));

		QObject::connect(UserInterfaceAccessor.SCLoadButton, SIGNAL(released()), SLOT(LoadScannerCalibrationPath()));
		QObject::connect(UserInterfaceAccessor.SCUpdateButton, SIGNAL(released()), SLOT(UpdateScannerCalibrationFromPath()));
	}

	// Hyper Parameter
	{
		RedTh = 0;
		WindowSize = 1;
		LimitXY = 1.0f;
		LimitZ = 1.0f;
		OdometryCC = 0.0f;
		MeasurementCC = 1.0f;

		QObject::connect(UserInterfaceAccessor.RThSpinBox, SIGNAL(valueChanged(int)), SLOT(UpdateRedThreshold(int)));
		QObject::connect(UserInterfaceAccessor.WSizeSpinBox, SIGNAL(valueChanged(int)), SLOT(UpdateWindowSize(int)));
		
		QObject::connect(UserInterfaceAccessor.LimXYSlider, SIGNAL(valueChanged(int)), SLOT(UpdateLimitXY(int)));
		QObject::connect(UserInterfaceAccessor.LimZSlider, SIGNAL(valueChanged(int)), SLOT(UpdateLimitZ(int)));
		QObject::connect(UserInterfaceAccessor.OdoCCSlider, SIGNAL(valueChanged(int)), SLOT(UpdateOdometryCharacteristicConstant(int)));
		QObject::connect(UserInterfaceAccessor.MeaCCSlider, SIGNAL(valueChanged(int)), SLOT(UpdateMeasurementCharacteristicConstant(int)));
	}
}

/** AScanner�� �����͸� �޾� ���� ������ ����մϴ�. */
void AUserInterface::SetScannerReference(AScanner* ScannerReference)
{
	Scanner = ScannerReference;

}

/** TimeManager�� ���� Delta Time�� ms ������ ��ȯ�մϴ�. */
int AUserInterface::GetDeltaTime()
{
	int DeltaTime = TimeManager.elapsed();

	return DeltaTime;
}

/** ������� Step motor�� Step Ƚ���� ��ȯ�մϴ�. */
int AUserInterface::GetStepCount()
{

	return StepCount;
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
#if AUSERINTERFACE_DEBUG == true
		cout << "Timer Init!" << endl;
#endif
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
		TimeManager.start();

		char SendData[] = "<X001!";
		SendData[1] = 10;
		SerialPort->write(SendData);
		StepCount++;
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

/** ���� Plane Parameter�� Save �մϴ�. */
void AUserInterface::SaveScannerCalibrationData()
{
	ofstream File("ScannerCalibration.txt");
	if (File.is_open() && (Scanner != nullptr))
	{
		Mat PlaneParameter = Scanner->LineLaser->GetPlaneParams();
		double** PlaneParamArray;
		AScannerHelper::SquareArrayDynamicAllocate(1, 4, PlaneParamArray);
		AScannerHelper::MatToArray(PlaneParameter, 1, 4, PlaneParamArray);

		for (int Index = 0; Index < 4; Index++)
		{
			File << PlaneParamArray[0][Index] << " ";
		}

		AScannerHelper::SquareArrayAllocateFree(1, 4, PlaneParamArray);
		File.close();

#if AUSERINTERFACE_DEBUG == true
		cout << "���������� Scanner Calibration File�� ����Ǿ����ϴ�." << endl;
#endif
	}
}

/** Camera Calibration File�� Path�� Load�մϴ�. */
void AUserInterface::LoadCameraCalibrationPath()
{
	CameraCalibrationPath = QFileDialog::getOpenFileName(
		this,
		tr("Open File"),
		".",
		"All files (*.*);;Text File (*.txt*)"
	);
	UserInterfaceAccessor.CCPathText->setText(CameraCalibrationPath);
}

/** Load�� Camera Calibration Path�� ������� Update�մϴ�. */
void AUserInterface::UpdateCameraCalibrationFromPath()
{
	if (Scanner != nullptr)
	{
		Scanner->ScanCamera->ReadCameraParameters(CameraCalibrationPath.toStdString());
	}
}

/** Scanner Calibration File�� Path�� Load�մϴ�. */
void AUserInterface::LoadScannerCalibrationPath()
{
	ScannerCalibrationPath = QFileDialog::getOpenFileName(
		this,
		tr("Open File"),
		".",
		"All files (*.*);;Text File (*.txt*)"
	);
	UserInterfaceAccessor.SCPathText->setText(ScannerCalibrationPath);
}

/** Load�� Scanner Calibration Path�� ������� Update�մϴ�. */
void AUserInterface::UpdateScannerCalibrationFromPath()
{
	ifstream File(ScannerCalibrationPath.toStdString());
	if (File.is_open() && (Scanner != nullptr))
	{
		std::string FileString;
		while (getline(File, FileString))
		{
			File >> FileString;
		}

		double PlaneParamArray[4];
		vector<string> SplitedString = SplitByCharacter(FileString, ' ');
		for (int Index = 0; Index < SplitedString.size(); Index++)
		{
			PlaneParamArray[Index] = stod(SplitedString[Index]);
#if AUSERINTERFACE_DEBUG == true
			cout << PlaneParamArray[Index] << " ";
#endif
		}
		
		Mat PlaneParameter = AScannerHelper::GetPlaneParameter(PlaneParamArray);
		Scanner->LineLaser->SetPlaneParams(PlaneParameter);
		SetLaserPlane(PlaneParameter);
		File.close();
	}
	
}

/** Label Type�� Frame ������ �Է¹޾� �ش� Label�� Update�մϴ�. */
void AUserInterface::UpdateLabelFromFrame(ELabelType LabelType, Mat& Frame)
{
	switch (LabelType)
	{
		case ELabelType::RGBFrame:
		{
			QSize LabelSize = UserInterfaceAccessor.LabelRGBFrame->size();
			cv::resize(Frame, Frame, cv::Size(LabelSize.width(), LabelSize.height()));
			cvtColor(Frame, Frame, CV_BGR2RGB);
			QImage Image = QImage(Frame.data, Frame.cols, Frame.rows, Frame.step, QImage::Format_RGB888);
			UserInterfaceAccessor.LabelRGBFrame->setPixmap(QPixmap::fromImage(Image));

			break;
		}
		case ELabelType::LaserFrame:
		{
			QSize LabelSize = UserInterfaceAccessor.LabelLaserFrame->size();
			cv::resize(Frame, Frame, cv::Size(LabelSize.width(), LabelSize.height()));
			QImage Image = QImage(Frame.data, Frame.cols, Frame.rows, Frame.step, QImage::Format_Grayscale8);
			UserInterfaceAccessor.LabelLaserFrame->setPixmap(QPixmap::fromImage(Image));

			break;
		}
		default:
		{
			// Error
			break;
		}

	}
}

/** Spin Value ���� ���� ������� Redth ���� Update �մϴ�. */
void AUserInterface::UpdateRedThreshold(int SpinValue)
{
	RedTh = SpinValue;
#if AUSERINTERFACE_DEBUG
	cout << "RedTh : " << RedTh << endl;
#endif
}

/** Spin Value ���� ���� ������� Anti Aliasing�� �����Ǵ� Window Size ���� Update �մϴ�. */
void AUserInterface::UpdateWindowSize(int SpinValue)
{
	WindowSize = SpinValue;
#if AUSERINTERFACE_DEBUG
	cout << "WindowSize : " << WindowSize << endl;
#endif
}

/** Slider�� ���� ���� ������� ���� ���� Scan Volume�� ����ġ�� Update �մϴ�. */
void AUserInterface::UpdateLimitXY(int SlideValue)
{
	LimitXY = SlideValue / 100.0f;
	UserInterfaceAccessor.LimXYText->setText(QString::number(LimitXY));
#if AUSERINTERFACE_DEBUG
	cout << "LimitXY : " << LimitXY << endl;
#endif
}
/** Slider�� ���� ���� ������� ���� ���� Scan Volume�� ����ġ�� Update �մϴ�. */
void AUserInterface::UpdateLimitZ(int SlideValue)
{
	LimitZ = SlideValue / 100.0f;
	UserInterfaceAccessor.LimZText->setText(QString::number(LimitZ));
#if AUSERINTERFACE_DEBUG
	cout << "LimitZ : " << LimitZ << endl;
#endif
}

/** Slider�� ���� ���� ������� Odometry model�� �л꿡 �����ϴ� Ư�� ��� ���� Update �մϴ�. */
void AUserInterface::UpdateOdometryCharacteristicConstant(int SlideValue)
{
	OdometryCC = SlideValue / 100.0f;
	UserInterfaceAccessor.OdoCCText->setText(QString::number(OdometryCC));
#if AUSERINTERFACE_DEBUG
	cout << "OdometryCC : " << OdometryCC << endl;
#endif
}

/** Slider�� ���� ���� ������� Measurement model�� �л꿡 �����ϴ� Ư�� ��� ���� Update �մϴ�. */
void AUserInterface::UpdateMeasurementCharacteristicConstant(int SlideValue)
{
	MeasurementCC = SlideValue / 100.0f;
	UserInterfaceAccessor.MeaCCText->setText(QString::number(MeasurementCC));
#if AUSERINTERFACE_DEBUG
	cout << "MeasurementCC : " << MeasurementCC << endl;
#endif
}

/** Reference�� Scan Data�� �ʱ�ȭ�մϴ�. */
void AUserInterface::ClearReferenceScanData()
{
#if AUSERINTERFACE_DEBUG
	cout << "Scan Data Clear ����� ���ŵǾ����ϴ�." << endl;
#endif
	if (Scanner != nullptr)
	{
		Scanner->ClearScanData();
		StepCount = 0;
	}
}

/** Reference�� Scan Data�� Processing�Ͽ� Draw�մϴ�. */
void AUserInterface::ProcessingFromReferenceScanData()
{
#if AUSERINTERFACE_DEBUG
	cout << "(�̱���) Draw OpenGL" << endl;
#endif
}

/** Scanner�� �ʱ� Transform ������ ����մϴ�. */
void AUserInterface::InitializeScanTransform()
{
	
}

AUserInterface::~AUserInterface()
{
	delete SerialPort;
}
