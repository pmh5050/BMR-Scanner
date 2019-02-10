#include "AScanner.h"

#include "AScanCamera.h"
#include "ACheckerBoard.h"
#include "ALineLaser.h"
#include "AUserInterface.h"

#include "AScannerHelper.h"

#include "ALinkedList.h"

int main(int argc, char *argv[])
{
	AScanner Scanner(argc, argv);
	Mat Frame;
	Mat Gray;


	while (Scanner.ScanCamera->IsFrameReady())
	{
		// Visible behavior
		switch (Scanner.UserInterface->VisibleBoxInfo)
		{
		case 1:
			Scanner.ScanCamera->ShowFrame();
			break;
		case 2:
			Scanner.SetObjectCheckerBoard('H');
			Scanner.DetectCheckerBoard(Frame);
			Scanner.DrawCheckerBoardAxis(Frame);
			imshow("Frame", Frame);
			break;
		case 4:
			Scanner.SetObjectCheckerBoard('V');
			Scanner.DetectCheckerBoard(Frame);
			Scanner.DrawCheckerBoardAxis(Frame);
			imshow("Frame", Frame);
			break;
		case 8:
			Scanner.ScanCamera->GetFrame(Frame);
			Gray = Scanner.ScannerHelper->GetLineLaserFrame(Frame, Scanner.LineLaser->GetMinimumRedValueThreshold());
			imshow("Laser Line", Gray);
			break;
		case 16:
			Scanner.SetObjectCheckerBoard('H');
			Scanner.DetectCheckerBoard(Frame);
			Scanner.UpdateCheckerBoardTransformMatrix();
			Scanner.UpdateCheckerBoardParams();

			if (Scanner.IsValidPose())
			{
				imshow("Frame", Frame);
				Scanner.UpdateCursorData();

				int x = Scanner.CurrentCursorData.x;
				int y = Scanner.CurrentCursorData.y;

				double dRayVector[3];
				Scanner.ScannerHelper->CalRayVector(x, y, Scanner.ScanCamera, dRayVector);

				Mat RayVector(1, 3, CV_64F, dRayVector);
				Mat cIntersectionPoint = Scanner.ScannerHelper->CalIntersectionPoint(Scanner.GetObjectCheckerBoard()->GetPlaneParams(), RayVector);
				Mat bIntersectionPoint = Scanner.GetObjectCheckerBoard()->GetTransformB2C().inv() * cIntersectionPoint;
				system("cls");
				cout << "Vector : " << RayVector << endl;
				cout << "Point : " << bIntersectionPoint << endl;
			}
			break;
		default:
			break;
		}

		// Update behavior
		switch (Scanner.UserInterface->UpdateBoxInfo)
		{
		case 1:
			Scanner.SetObjectCheckerBoard('H');
			Scanner.DetectCheckerBoard(Frame);
			Scanner.UpdateCheckerBoardTransformMatrix();
			Scanner.UpdateCheckerBoardParams();
			Scanner.UpdateCheckerBoardLaserLine(Frame);
			break;
		case 2:
			Scanner.SetObjectCheckerBoard('V');
			Scanner.DetectCheckerBoard(Frame);
			Scanner.UpdateCheckerBoardTransformMatrix();
			Scanner.UpdateCheckerBoardParams();
			Scanner.UpdateCheckerBoardLaserLine(Frame);
			break;
		case 4: // Update Line Laser
			Scanner.UpdateLineLaserPlaneParams();
			break;
		default:
			break;
		}

		// Scan behavior
		{
			if (Scanner.UserInterface->ScanBoxInfo & (1 << SCAN_RUN))
			{
				Scanner.ScanRunning();
			}

			if (Scanner.UserInterface->ScanBoxInfo & (1 << SCAN_EXPORT))
			{
				cout << "Export" << endl;
				break;
			}
		}
		
		waitKey(1);
	}

	if (Scanner.IsScanDataReady())
	{
		cout << "Computing for Scan Data" << endl;
		cout << "Please wait" << endl;
		
		double ScanVolume[3][2];

		ScanVolume[X_AXIS][RANGE_MINIMUM] = 0.038f;
		ScanVolume[X_AXIS][RANGE_MAXIMUM] = 0.152f;

		ScanVolume[Y_AXIS][RANGE_MINIMUM] = 0.076f;
		ScanVolume[Y_AXIS][RANGE_MAXIMUM] = 0.190f;
		
		ScanVolume[Z_AXIS][RANGE_MINIMUM] = 0.01f;
		ScanVolume[Z_AXIS][RANGE_MAXIMUM] = 0.0685f; 

		Scanner.StorePointCloud("PointCloud.txt", ScanVolume);

		cout << "Complete the task" << endl;
	}
	else
	{
		cout << "Scan data doesn't exist" << endl;
	}
	Scanner.LinkedListHead->DestroyNode();
	cout << "Program이 정상적으로 종료되었습니다." << endl;
	return Scanner.Application->exec();
}
