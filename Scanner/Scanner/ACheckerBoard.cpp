#include "ACheckerBoard.h"

#define ACHECKERBOARD_DEBUG true

#if ACHECKERBOARD_DEBUG
#include <iostream>
using namespace std;
#endif

ACheckerBoard::ACheckerBoard(int NewSquaresX, int NewSquaresY, float NewSquareLength, float NewMarkerLength, int NewDictionaryId)
{
	SquaresX = NewSquaresX; // CheckerBoard의 가로 개수
	SquaresY = NewSquaresY; // CheckerBoard의 세로 개수
	SquareLength = NewSquareLength; // CheckerBoard 사각형 한 변의 길이(m)
	MarkerLength = NewMarkerLength; // CheckerBoard marker 한 변의 길이
	DictionaryId = NewDictionaryId; // ArUco marker Id

	InitACheckerBoard();
}

/** 생성자로부터 결정된 Parameter들로 ArUco 변수 및 flag 등을 초기화합니다 */
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

/** Filename을 입력받아 DetectorParameters를 설정합니다*/
void ACheckerBoard::ReadDetectorParameters(string Filename)
{
	FileStorage Fs(Filename, FileStorage::READ);
	if (!Fs.isOpened())
	{
#if ACHECKERBOARD_DEBUG
		cout << "File을 읽는데 실패하였습니다." << endl;
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

/** Detector parameter File을 읽어들였는지 유무를 반환합니다 */
bool ACheckerBoard::IsReadDetectorParameters()
{
	return bIsReadDetectorParameters;
}

/** Mat 형식으로 B2C Transform Matrix를 받아서 저장합니다 */
void ACheckerBoard::SetTransformB2C(Mat NewTrasnformB2C)
{
	TransformB2C = NewTrasnformB2C.clone();
}

/** Mat 형식으로 B2C Transform Matrix를 반환합니다 */
Mat ACheckerBoard::GetTransformB2C()
{
	return TransformB2C;
}

/** Mat 형식으로 Camera Coordinate의 Normal Vector를 받아서 저장합니다 */
void ACheckerBoard::SetNormalVector(Mat NewNormalVector)
{
	NormalVector = NewNormalVector.clone();
}

/** Mat 형식으로 Camera Coordinate의 원점을 받아서 저장합니다 */
void ACheckerBoard::SetPivotPoint(Mat NewPivotPoint)
{
	PivotPoint = NewPivotPoint.clone();
}

/** Mat 형식으로 Board의 Normal Vector를 반환합니다 */
Mat ACheckerBoard::GetNormalVector()
{
	return NormalVector;
}

/** Mat 형식으로 Board의 원점을 반환합니다 */
Mat ACheckerBoard::GetPivotPoint()
{
	return PivotPoint;
}

/** Mat 형식으로 CheckerBoard의 Plane Parameter를 전달받아서 저장합니다 */
void ACheckerBoard::SetPlaneParams(Mat NewPlaneParams)
{
	PlaneParams = NewPlaneParams.clone();
}

/** Mat 형식으로 CheckerBoard의 Plane Parameter를 반환합니다 */
Mat ACheckerBoard::GetPlaneParams()
{
	return PlaneParams;
}

/** CheckerBoard에 투영된 Line direction Vector를 Mat형식으로 받아 저장합니다. */
void ACheckerBoard::SetLaserLineVector(Mat NewLineVector)
{
	LaserLineVector = NewLineVector.clone();
}

/** 저장된 Line direction Vector를 Mat형식으로 반환합니다. */
Mat ACheckerBoard::GetLaserLineVector()
{
	return LaserLineVector;
}

/** Laser Line Plane과 CheckerBoard의 교점 중 한 점의 위치를 입력받아 Laser Pivot Point에 저장합니다. */
void ACheckerBoard::SetLaserPivotPoint(Mat NewLaserPivotPoint)
{
	cout << "New Laser Pivot Point Updated!" << endl;
	LaserPivotPoint = NewLaserPivotPoint.clone();
}

/** Laser Line Plane과 CheckerBoard의 교점 중 한 점의 위치를 반환합니다 */
Mat ACheckerBoard::GetLaserPivotPoint()
{
	return LaserPivotPoint;
}