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
	
	// Visible Button Group 구성
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
	
	// Update Button Group 구성
	{
		UpdateButtonGroup = new QButtonGroup();

		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateHBoardBox, 0);
		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateVBoardBox, 1);
		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateLineLaserBox, 2);
		UpdateButtonGroup->addButton(UserInterfaceAccessor.UpdateNoneBox, 3);

		QObject::connect(UpdateButtonGroup, SIGNAL(buttonToggled(int, bool)), SLOT(UpdateBoxSlot(int, bool)));
	}

	// Information Button 구성
	{
		QObject::connect(UserInterfaceAccessor.SCSaveButton, SIGNAL(released()), SLOT(SaveScannerCalibrationData()));
	}

	// Scan Button Group 구성
	{
		ScanButtonGroup = new QButtonGroup();
		ScanButtonGroup->setExclusive(false);

		ScanButtonGroup->addButton(UserInterfaceAccessor.ScanBox, 0);
		ScanButtonGroup->addButton(UserInterfaceAccessor.ExportBox, 1);

		QObject::connect(ScanButtonGroup, SIGNAL(buttonToggled(int, bool)), SLOT(ScanBoxSlot(int, bool)));

		QObject::connect(UserInterfaceAccessor.ScanDataClearButton, SIGNAL(released()), SLOT(ClearReferenceScanData()));
		QObject::connect(UserInterfaceAccessor.ScanProcessingButton, SIGNAL(released()), SLOT(ProcessingFromReferenceScanData()));
	}

	// Serial Port Group 구성
	{
		bIsSerialOpened = false;
		StepCount = 0;

		SerialPort = new QSerialPort();
		QObject::connect(UserInterfaceAccessor.ConnectButton, SIGNAL(released()), SLOT(SerialConnect()));
		QObject::connect(UserInterfaceAccessor.UnitStepButton, SIGNAL(released()), SLOT(StepOnce()));
	}

	// Timer 구성
	{
		TimerHandler = new QTimer(this);
		bIsTimerRunning = false;

		QObject::connect(TimerHandler, SIGNAL(timeout()), SLOT(StepOnce()));

	}

	// Calibration 구성
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

/** AScanner의 포인터를 받아 참조 변수에 등록합니다. */
void AUserInterface::SetScannerReference(AScanner* ScannerReference)
{
	Scanner = ScannerReference;

}

/** TimeManager로 부터 Delta Time을 ms 단위로 반환합니다. */
int AUserInterface::GetDeltaTime()
{
	int DeltaTime = TimeManager.elapsed();

	return DeltaTime;
}

/** 현재까지 Step motor의 Step 횟수를 반환합니다. */
int AUserInterface::GetStepCount()
{

	return StepCount;
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
#if AUSERINTERFACE_DEBUG == true
		cout << "Timer Init!" << endl;
#endif
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
		TimeManager.start();

		char SendData[] = "<X001!";
		SendData[1] = 10;
		SerialPort->write(SendData);
		StepCount++;
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

/** 현재 Plane Parameter를 Save 합니다. */
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
		cout << "정상적으로 Scanner Calibration File이 저장되었습니다." << endl;
#endif
	}
}

/** Camera Calibration File의 Path를 Load합니다. */
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

/** Load한 Camera Calibration Path를 기반으로 Update합니다. */
void AUserInterface::UpdateCameraCalibrationFromPath()
{
	if (Scanner != nullptr)
	{
		Scanner->ScanCamera->ReadCameraParameters(CameraCalibrationPath.toStdString());
	}
}

/** Scanner Calibration File의 Path를 Load합니다. */
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

/** Load한 Scanner Calibration Path를 기반으로 Update합니다. */
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

/** Label Type과 Frame 정보를 입력받아 해당 Label을 Update합니다. */
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

/** Spin Value 조정 값을 기반으로 Redth 값을 Update 합니다. */
void AUserInterface::UpdateRedThreshold(int SpinValue)
{
	RedTh = SpinValue;
#if AUSERINTERFACE_DEBUG
	cout << "RedTh : " << RedTh << endl;
#endif
}

/** Spin Value 조정 값을 기반으로 Anti Aliasing에 참조되는 Window Size 값을 Update 합니다. */
void AUserInterface::UpdateWindowSize(int SpinValue)
{
	WindowSize = SpinValue;
#if AUSERINTERFACE_DEBUG
	cout << "WindowSize : " << WindowSize << endl;
#endif
}

/** Slider의 조정 값을 기반으로 수평 방향 Scan Volume의 제한치를 Update 합니다. */
void AUserInterface::UpdateLimitXY(int SlideValue)
{
	LimitXY = SlideValue / 100.0f;
	UserInterfaceAccessor.LimXYText->setText(QString::number(LimitXY));
#if AUSERINTERFACE_DEBUG
	cout << "LimitXY : " << LimitXY << endl;
#endif
}
/** Slider의 조정 값을 기반으로 수직 방향 Scan Volume의 제한치를 Update 합니다. */
void AUserInterface::UpdateLimitZ(int SlideValue)
{
	LimitZ = SlideValue / 100.0f;
	UserInterfaceAccessor.LimZText->setText(QString::number(LimitZ));
#if AUSERINTERFACE_DEBUG
	cout << "LimitZ : " << LimitZ << endl;
#endif
}

/** Slider의 조정 값을 기반으로 Odometry model의 분산에 관여하는 특성 상수 값을 Update 합니다. */
void AUserInterface::UpdateOdometryCharacteristicConstant(int SlideValue)
{
	OdometryCC = SlideValue / 100.0f;
	UserInterfaceAccessor.OdoCCText->setText(QString::number(OdometryCC));
#if AUSERINTERFACE_DEBUG
	cout << "OdometryCC : " << OdometryCC << endl;
#endif
}

/** Slider의 조정 값을 기반으로 Measurement model의 분산에 관여하는 특성 상수 값을 Update 합니다. */
void AUserInterface::UpdateMeasurementCharacteristicConstant(int SlideValue)
{
	MeasurementCC = SlideValue / 100.0f;
	UserInterfaceAccessor.MeaCCText->setText(QString::number(MeasurementCC));
#if AUSERINTERFACE_DEBUG
	cout << "MeasurementCC : " << MeasurementCC << endl;
#endif
}

/** Reference의 Scan Data를 초기화합니다. */
void AUserInterface::ClearReferenceScanData()
{
#if AUSERINTERFACE_DEBUG
	cout << "Scan Data Clear 명령이 수신되었습니다." << endl;
#endif
	if (Scanner != nullptr)
	{
		Scanner->ClearScanData();
		StepCount = 0;
	}
}

/** Reference의 Scan Data를 Processing하여 Draw합니다. */
void AUserInterface::ProcessingFromReferenceScanData()
{
#if AUSERINTERFACE_DEBUG
	cout << "(미구현) Draw OpenGL" << endl;
#endif
}

/** Scanner의 초기 Transform 정보를 등록합니다. */
void AUserInterface::InitializeScanTransform()
{
	
}

AUserInterface::~AUserInterface()
{
	delete SerialPort;
}
