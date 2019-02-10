#include "ACheckerBoard.h"

#define ACHECKERBOARD_DEBUG true

#if ACHECKERBOARD_DEBUG
#include <iostream>
using namespace std;
#endif

ACheckerBoard::ACheckerBoard(int NewSquaresX, int NewSquaresY, float NewSquareLength, float NewMarkerLength, int NewDictionaryId)
{
	SquaresX = NewSquaresX; // CheckerBoard�� ���� ����
	SquaresY = NewSquaresY; // CheckerBoard�� ���� ����
	SquareLength = NewSquareLength; // CheckerBoard �簢�� �� ���� ����(m)
	MarkerLength = NewMarkerLength; // CheckerBoard marker �� ���� ����
	DictionaryId = NewDictionaryId; // ArUco marker Id

	InitACheckerBoard();
}

/** �����ڷκ��� ������ Parameter��� ArUco ���� �� flag ���� �ʱ�ȭ�մϴ� */
void ACheckerBoard::InitACheckerBoard()
{
	bIsReadDetectorParameters = false;
	IsTransformReady = false;

	DetectorParams = aruco::DetectorParameters::create();
	Dictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(DictionaryId));
	AxisLength = 0.5f * ((float)min(SquaresX, SquaresY) * (SquareLength));
	Charucoboard = aruco::CharucoBoard::create(SquaresX, SquaresY, SquareLength, MarkerLength, Dictionary);
	Board = Charucoboard.staticCast<aruco::Board>();
}

ACheckerBoard::~ACheckerBoard()
{

}

/** Filename�� �Է¹޾� DetectorParameters�� �����մϴ�*/
void ACheckerBoard::ReadDetectorParameters(string Filename)
{
	FileStorage Fs(Filename, FileStorage::READ);
	if (!Fs.isOpened())
	{
#if ACHECKERBOARD_DEBUG
		cout << "File�� �дµ� �����Ͽ����ϴ�." << endl;
		cout << "File name : " << Filename << endl;
#endif
		bIsReadDetectorParameters = false;
	}
	else
	{
		Fs["adaptiveThreshWinSizeMin"] >> DetectorParams->adaptiveThreshWinSizeMin;
		Fs["adaptiveThreshWinSizeMax"] >> DetectorParams->adaptiveThreshWinSizeMax;
		Fs["adaptiveThreshWinSizeStep"] >> DetectorParams->adaptiveThreshWinSizeStep;
		Fs["adaptiveThreshConstant"] >> DetectorParams->adaptiveThreshConstant;
		Fs["minMarkerPerimeterRate"] >> DetectorParams->minMarkerPerimeterRate;
		Fs["maxMarkerPerimeterRate"] >> DetectorParams->maxMarkerPerimeterRate;
		Fs["polygonalApproxAccuracyRate"] >> DetectorParams->polygonalApproxAccuracyRate;
		Fs["minCornerDistanceRate"] >> DetectorParams->minCornerDistanceRate;
		Fs["minDistanceToBorder"] >> DetectorParams->minDistanceToBorder;
		Fs["minMarkerDistanceRate"] >> DetectorParams->minMarkerDistanceRate;
		Fs["cornerRefinementMethod"] >> DetectorParams->cornerRefinementMethod;
		Fs["cornerRefinementWinSize"] >> DetectorParams->cornerRefinementWinSize;
		Fs["cornerRefinementMaxIterations"] >> DetectorParams->cornerRefinementMaxIterations;
		Fs["cornerRefinementMinAccuracy"] >> DetectorParams->cornerRefinementMinAccuracy;
		Fs["markerBorderBits"] >> DetectorParams->markerBorderBits;
		Fs["perspectiveRemovePixelPerCell"] >> DetectorParams->perspectiveRemovePixelPerCell;
		Fs["perspectiveRemoveIgnoredMarginPerCell"] >> DetectorParams->perspectiveRemoveIgnoredMarginPerCell;
		Fs["maxErroneousBitsInBorderRate"] >> DetectorParams->maxErroneousBitsInBorderRate;
		Fs["minOtsuStdDev"] >> DetectorParams->minOtsuStdDev;
		Fs["errorCorrectionRate"] >> DetectorParams->errorCorrectionRate;
		bIsReadDetectorParameters = true;
	}
}

/** Detector parameter File�� �о�鿴���� ������ ��ȯ�մϴ� */
bool ACheckerBoard::IsReadDetectorParameters()
{
	return bIsReadDetectorParameters;
}

/** Mat �������� B2C Transform Matrix�� �޾Ƽ� �����մϴ� */
void ACheckerBoard::SetTransformB2C(Mat NewTrasnformB2C)
{
	TransformB2C = NewTrasnformB2C.clone();
}

/** Mat �������� B2C Transform Matrix�� ��ȯ�մϴ� */
Mat ACheckerBoard::GetTransformB2C()
{
	return TransformB2C;
}

/** Mat �������� Camera Coordinate�� Normal Vector�� �޾Ƽ� �����մϴ� */
void ACheckerBoard::SetNormalVector(Mat NewNormalVector)
{
	NormalVector = NewNormalVector.clone();
}

/** Mat �������� Camera Coordinate�� ������ �޾Ƽ� �����մϴ� */
void ACheckerBoard::SetPivotPoint(Mat NewPivotPoint)
{
	PivotPoint = NewPivotPoint.clone();
}

/** Mat �������� Board�� Normal Vector�� ��ȯ�մϴ� */
Mat ACheckerBoard::GetNormalVector()
{
	return NormalVector;
}

/** Mat �������� Board�� ������ ��ȯ�մϴ� */
Mat ACheckerBoard::GetPivotPoint()
{
	return PivotPoint;
}

/** Mat �������� CheckerBoard�� Plane Parameter�� ���޹޾Ƽ� �����մϴ� */
void ACheckerBoard::SetPlaneParams(Mat NewPlaneParams)
{
	PlaneParams = NewPlaneParams.clone();
}

/** Mat �������� CheckerBoard�� Plane Parameter�� ��ȯ�մϴ� */
Mat ACheckerBoard::GetPlaneParams()
{
	return PlaneParams;
}

/** CheckerBoard�� ������ Line direction Vector�� Mat�������� �޾� �����մϴ�. */
void ACheckerBoard::SetLaserLineVector(Mat NewLineVector)
{
	LaserLineVector = NewLineVector.clone();
}

/** ����� Line direction Vector�� Mat�������� ��ȯ�մϴ�. */
Mat ACheckerBoard::GetLaserLineVector()
{
	return LaserLineVector;
}

/** Laser Line Plane�� CheckerBoard�� ���� �� �� ���� ��ġ�� �Է¹޾� Laser Pivot Point�� �����մϴ�. */
void ACheckerBoard::SetLaserPivotPoint(Mat NewLaserPivotPoint)
{
	cout << "New Laser Pivot Point Updated!" << endl;
	LaserPivotPoint = NewLaserPivotPoint.clone();
}

/** Laser Line Plane�� CheckerBoard�� ���� �� �� ���� ��ġ�� ��ȯ�մϴ� */
Mat ACheckerBoard::GetLaserPivotPoint()
{
	return LaserPivotPoint;
}