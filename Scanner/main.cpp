#include "AScanner.h"
#include "AScanCamera.h"
#include "ACheckerBoard.h"
#include "ALineLaser.h"
#include "AUserInterface.h"
#include "AScannerHelper.h"
#include "ALinkedList.h"

#define MAIN_DEBUG false

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define RANGE_MINIMUM 0
#define RANGE_MAXIMUM 1

int main(int argc, char *argv[])
{
	AScanner Scanner(argc, argv);
	Mat Frame;
	Mat LaserFrame;
	
	while (Scanner.ScanCamera->IsFrameReady())
	{
		// Visible behavior
		{
			if (Scanner.UserInterface->VisibleBoxInfo & (1 << VISIBLE_DEFAULT))
			{
				Scanner.ScanCamera->ShowFrame();
			}

			if (Scanner.UserInterface->VisibleBoxInfo & (1 << VISIBLE_H_BOARD))
			{
				Scanner.SetObjectCheckerBoard('H');
				Scanner.DetectCheckerBoard(Frame);
				Scanner.DrawCheckerBoardAxis(Frame);
				imshow("Frame", Frame);
			}

			if (Scanner.UserInterface->VisibleBoxInfo & (1 << VISIBLE_V_BOARD))
			{
				Scanner.SetObjectCheckerBoard('V');
				Scanner.DetectCheckerBoard(Frame);
				Scanner.DrawCheckerBoardAxis(Frame);
				imshow("Frame", Frame);
			}

			if (Scanner.UserInterface->VisibleBoxInfo & (1 << VISIBLE_LINE_LASER))
			{
				Scanner.ScanCamera->GetFrame(Frame);
				LaserFrame = AScannerHelper::GetLineLaserFrame(Frame, Scanner.LineLaser->GetMinimumRedValueThreshold());
				imshow("Laser Frame", LaserFrame);
			}

			if (Scanner.UserInterface->VisibleBoxInfo & (1 << VISIBLE_CURSOR_POSITION))
			{
				Scanner.SetObjectCheckerBoard('H');
				Scanner.DetectCheckerBoard(Frame);
				Scanner.UpdateCheckerBoardTransformMatrix();
				Scanner.UpdateCheckerBoardParams();
				imshow("Frame", Frame);

				if (Scanner.IsValidPose())
				{
					Scanner.UpdateCursorData();

					int CursorX = Scanner.CurrentCursorData.x;
					int CursorY = Scanner.CurrentCursorData.y;

					double RayVectorArray[3];
					AScannerHelper::CalRayVector(CursorX, CursorY, Scanner.ScanCamera, RayVectorArray);

					Mat RayVector(1, 3, CV_64F, RayVectorArray);
					Mat cIntersectionPoint = AScannerHelper::CalIntersectionPoint(Scanner.GetObjectCheckerBoard()->GetPlaneParams(), RayVector);
					Mat bIntersectionPoint = Scanner.GetObjectCheckerBoard()->GetTransformB2C().inv() * cIntersectionPoint;
					system("cls");
					cout << "Vector : " << RayVector << endl;
					cout << "Point : " << bIntersectionPoint << endl;
				}
			}
		}

		// Update behavior
		{
			if (Scanner.UserInterface->UpdateBoxInfo & (1 << UPDATE_H_BOARD))
			{
				Scanner.SetObjectCheckerBoard('H');
				Scanner.DetectCheckerBoard(Frame);
				Scanner.UpdateCheckerBoardTransformMatrix();
				Scanner.UpdateCheckerBoardParams();
				Scanner.UpdateCheckerBoardLaserLine(Frame);
			}

			if (Scanner.UserInterface->UpdateBoxInfo & (1 << UPDATE_V_BOARD))
			{
				Scanner.SetObjectCheckerBoard('V');
				Scanner.DetectCheckerBoard(Frame);
				Scanner.UpdateCheckerBoardTransformMatrix();
				Scanner.UpdateCheckerBoardParams();
				Scanner.UpdateCheckerBoardLaserLine(Frame);
			}

			if (Scanner.UserInterface->UpdateBoxInfo & (1 << UPDATE_LINE_LASER))
			{
				Scanner.UpdateLineLaserPlaneParams(); // Line Laser's plane update
			}

			if (Scanner.UserInterface->UpdateBoxInfo & (1 << UPDATE_NONE))
			{
				// No task
			}
		}

		// Scan behavior
		{
			if (Scanner.UserInterface->ScanBoxInfo & (1 << SCAN_RUN))
			{
				Scanner.ScanRunning();
			}

			if (Scanner.UserInterface->ScanBoxInfo & (1 << SCAN_EXPORT))
			{
#if MAIN_DEBUG
				cout << "Scan을 종료합니다." << endl;
#endif
				break;
			}
		}
		waitKey(1);
	}

	if (Scanner.IsScanDataReady())
	{
		cout << "Scan 데이터에 대한 연산을 시작합니다." << endl;
		cout << "잠시만 기다려주십시오." << endl;

		double ScanVolume[3][2]; // Scan data post processing volume

		ScanVolume[X_AXIS][RANGE_MINIMUM] = -0.05f;
		ScanVolume[X_AXIS][RANGE_MAXIMUM] = 0.05f;
		ScanVolume[Y_AXIS][RANGE_MINIMUM] = -0.05f;
		ScanVolume[Y_AXIS][RANGE_MAXIMUM] = +0.05f;
		ScanVolume[Z_AXIS][RANGE_MINIMUM] = 0.0015f;
		ScanVolume[Z_AXIS][RANGE_MAXIMUM] = 0.100f; 

		Scanner.StorePointCloud("PointCloud.txt", ScanVolume);

		cout << "모든 연산이 완료되었습니다." << endl;
	}
	else
	{
		cout << "Scan 데이터가 존재하지 않습니다." << endl;
	}

#if MAIN_DEBUG
	cout << "생성하였던 Node들을 반납합니다." << endl;
#endif
	Scanner.LinkedListHead->DestroyNode();

	cout << "Program이 정상적으로 종료되었습니다." << endl;

	return Scanner.Application->exec();
}
