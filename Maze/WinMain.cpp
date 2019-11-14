#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include "Maze.h"
#include <chrono>
#include "Object.h"
#include "Controller.h"
#define camOffX (mX+camX)
#define camOffY (mY+camY)
int WinMain(HINSTANCE hInst, HINSTANCE prev, LPTSTR cmd, int mode);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK drawProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LPSTR WinName = "MainWndClss";
Maze maze; ObjArr objects; int objectsCount = 10; Controller contr;
HWND MainWindow,drawW, LBObjects,CBMazeTile;
float x = 0, y = 0;static const int windW = 1000, windH = 800;
const RECT drawRect = {0,0,800,800};
// control
int mapmode; enum mapmodes { nothing, settile, addObject }; char parameters[32];
//
void(*updatePtr)(float); void(*drawPtr)(HDC hdc); void(*mouseEventPtr)(UINT msg); int mode = 0;
int fldSize = 20;
int mX = 0, mY = 0, dmX = 0, dmY = 0; bool mPress = false, mrX = 0, mrY = 0; bool mrPress = false;// mouse
int camX=0, camY=0; //camera
static HDC hdc, hdcB;
static HBITMAP hMemBMP;
static HGDIOBJ hOld;
PAINTSTRUCT ps;
static HBRUSH br;
void RectangleM(HDC hdc, int left, int top, int right, int bottom) {
	Rectangle(hdc, left- camX, top- camY, right-camX, bottom-camY);
}
void EllipseM(_In_ HDC hdc, _In_ int left, _In_ int top, _In_ int right, _In_ int bottom) {
	Ellipse(hdc, left - camX, top - camY, right - camX, bottom - camY);
}
void draw() {
	hdcB = BeginPaint(drawW, &ps);
	SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	Rectangle(hdc, 0, 0, windW, windH);
	drawPtr(hdc);
	SelectObject(hdc, hOld);
	BitBlt(hdcB, 0, 0, windW, windH, hdc, 0, 0, SRCCOPY);
	EndPaint(drawW, &ps);
}
void drawPM(HDC hdc) {
	SelectObject(hdc, GetStockObject(DC_PEN)); SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, RGB(100, 50, 0)); SetDCPenColor(hdc, RGB(100, 50, 0));
	for (int i = 0; i < contr.getMaze().GetHeight(); i++) {
		for (int j = 0; j < contr.getMaze().GetWidth(); j++) {
			if (contr.getMaze().GetField(j, i) == fldType::EMPTY) {
			}
			else {
				RectangleM(hdc, j * fldSize, i * fldSize, j * fldSize + fldSize, i * fldSize + fldSize);
			}
		}
	}
	SetDCBrushColor(hdc, RGB(100, 50, 150)); SetDCPenColor(hdc, RGB(100, 50, 0));
	for (int i = 0; i < contr.getObjects().getCount(); i++) {
			EllipseM(hdc, contr.getObjects()[i].GetXY(0)* fldSize - contr.getObjects()[i].getSize()+ fldSize/2, contr.getObjects()[i].GetXY(1)* fldSize - contr.getObjects()[i].getSize() + fldSize / 2, contr.getObjects()[i].GetXY(0)* fldSize + fldSize/2 + contr.getObjects()[i].getSize(), contr.getObjects()[i].GetXY(1)* fldSize + fldSize/2 + contr.getObjects()[i].getSize());
	}
	
}
void drawCM(HDC hdc) {
	SelectObject(hdc, GetStockObject(DC_PEN)); SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, RGB(100, 50, 0));SetDCPenColor(hdc, RGB(100, 50, 0));
	HRGN hr;
	for (int i = 0; i < maze.GetHeight(); i++) {
		for (int j = 0; j < maze.GetWidth(); j++) {
			if (maze.GetField(j, i) == fldType::EMPTY) {
			}
			else {
				RectangleM(hdc, j * fldSize, i * fldSize, j * fldSize + fldSize, i * fldSize + fldSize);
			}
		}
	}
	SetDCBrushColor(hdc, RGB(100, 50, 150)); SetDCPenColor(hdc, RGB(100, 50, 0));
	for (int i = 0; i < objects.getCount(); i++) {
		EllipseM(hdc, objects[i].GetXY(0)* fldSize - objects[i].getSize() + fldSize / 2, objects[i].GetXY(1)* fldSize - objects[i].getSize() + fldSize / 2, objects[i].GetXY(0)* fldSize + fldSize / 2 + objects[i].getSize(), objects[i].GetXY(1)* fldSize + fldSize / 2 + objects[i].getSize());
	}
	SetDCBrushColor(hdc, RGB(0, 255, 0));SetDCPenColor(hdc, RGB(0, 255, 0));
	MoveToEx(hdc, camOffX / fldSize * fldSize - camX, camOffY / fldSize * fldSize - camY,0);
	LineTo(hdc, camOffX / fldSize * fldSize + fldSize - camX, camOffY / fldSize * fldSize - camY);
	LineTo(hdc, camOffX / fldSize * fldSize + fldSize - camX, camOffY / fldSize * fldSize + fldSize - camY);
	LineTo(hdc, camOffX / fldSize * fldSize  - camX, camOffY / fldSize * fldSize + fldSize - camY);
	LineTo(hdc, camOffX / fldSize * fldSize - camX, camOffY / fldSize * fldSize - camY);
}
void updatePM(float dtm) {
	contr.update(dtm);
	InvalidateRect(drawW, NULL, 1);
}
void updateCM(float dtm) {
	InvalidateRect(drawW, NULL, 1);
}
void mouseEventCM(UINT msg) {
	switch (msg) {
	case WM_MOUSEMOVE:
		if (mapmode != nothing && mPress) {
			if (mapmode == settile) { maze.SetField(camOffX / fldSize, camOffY / fldSize, parameters[0]); }
			
		}else if (mrPress) {
			camX += dmX; camY += dmY;
		}
		break;
	case WM_LBUTTONDOWN:
		if (mapmode != nothing) {
			if (mapmode == settile) { maze.SetField(camOffX / fldSize, camOffY / fldSize, parameters[0]); }
			else if (mapmode == addObject) {
				if (maze.GetField(camOffX / fldSize, camOffY / fldSize) != fldType::WALL) {
					objects.push_back(*(new MazeObject(camOffX / fldSize, camOffY / fldSize)));
					SendMessage(LBObjects, LB_ADDSTRING, 0, (LPARAM)objects[objects.getCount()-1].objName);
				}
			}
		}
		break;
	}
}
void mouseEventPM(UINT msg) {
	switch (msg) {
	case WM_MOUSEMOVE:
		if (mrPress) {
			camX += dmX; camY += dmY;
		}
		break;
	}
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prev, LPTSTR cmd, int mode) {
	MSG msg;   
	WNDCLASS wc;   
	wc.hInstance = hInst;
	wc.lpszClassName = WinName;     
	wc.lpfnWndProc = WndProc;        
	wc.style = CS_HREDRAW | CS_VREDRAW;   
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);  
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wc.lpszMenuName = NULL; 
	wc.cbClsExtra = 0;    
	wc.cbWndExtra = 0;  
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW );	
	if (!RegisterClass(&wc))return 0;
	wc.lpszClassName = "drawWind_34526";
	wc.lpfnWndProc = drawProc;
	if (!RegisterClass(&wc))return 0;
	MainWindow = CreateWindow(WinName, "Maze", WS_CAPTION, CW_USEDEFAULT, CW_USEDEFAULT, windW, windH, NULL, NULL, hInst, NULL);
	ShowWindow(MainWindow, mode);
	UpdateWindow(MainWindow);
	bool done = true;
	auto start = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed;
	float frameTime = 0; const float frameM = 0.02f;
	updatePtr = updateCM; drawPtr = drawCM; mouseEventPtr = mouseEventCM;
	while (done) {
		elapsed = std::chrono::high_resolution_clock::now() - start;
		start = std::chrono::high_resolution_clock::now();
		if ((frameTime += elapsed.count())>frameM) {
			updatePtr(frameTime);
			draw();
			frameTime = 0;
		}
		else {
			Sleep(1);
		}

		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			if (msg.message == WM_QUIT)done = 0;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	RECT r, rs;
	char buff[50]; int intbuff[15]; char* tempC;
	static HWND htemp, EDMazeSizeX, EDMazeSizeY, EDFieldSize, GRSetsize, GRObject, EDEventChance[3], CBEventActions[3], EDObjectSize, EDObjectName, EDObjectSpeed,hBTStart, hBTSetobjectParam,hBTDelete,hBTSetSize;
	const int BTSetSize = 0, BTDeleteObject = 1, BTSetobjectParam = 2, BTStart = 3;
	std::string s;
	MazeObject obj;
	static int ran = 0;
	switch (msg) {
	case WM_CREATE:
		drawW = CreateWindow("drawWind_34526", "add", WS_CHILD | WS_VISIBLE | WS_BORDER, drawRect.left, drawRect.top, drawRect.right, drawRect.bottom, hWnd, 0, 0, 0);
		maze.randomFill(++ran);
		hdcB= GetDC(drawW);
		br = CreateSolidBrush(RGB(100, 50, 0));
		hdc = CreateCompatibleDC(hdcB);
		hMemBMP = CreateCompatibleBitmap(hdcB, windW, windH);
		SelectObject(hdc, hMemBMP);
		ReleaseDC(drawW, hdcB);
		//Maze
		hBTStart = CreateWindow("button", "Start", WS_CHILD | WS_VISIBLE | WS_BORDER, drawRect.right,5, 180, 25, hWnd, (HMENU)BTStart, 0, 0);
		GetWindowRect(hBTStart, &r); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&r, 2);
		CBMazeTile=CreateWindow("ComboBox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, drawRect.right, r.bottom+5, 100, 200, hWnd, (HMENU)111, 0, NULL);
		for (int i = 0; i < fldType::count; i++) {
			SendMessage(CBMazeTile, CB_ADDSTRING, 0, (LPARAM)fldType::Names[i]);
		}
		GetWindowRect(CBMazeTile, &r); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&r, 2);
		CreateWindow("static", "Maze Tile", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, r.right+ 5, r.top + 5, 130, 25, hWnd, 0, 0, 0);
		GetWindowRect(CBMazeTile, &r); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&r, 2);
		GRSetsize = CreateWindow("button", "maze size", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_GROUPBOX, drawRect.right+3, r.bottom + 5, 180, 140, hWnd, (HMENU)-1, 0, 0);
		GetWindowRect(GRSetsize, &rs); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&rs, 2);
		EDMazeSizeX = CreateWindow("edit", std::to_string(maze.GetWidth()).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rs.left+ 15, rs.top+20, 50, 25, hWnd, (HMENU)3, 0, 0);
		GetWindowRect(EDMazeSizeX, &r);
		CreateWindow("static", "Width", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, rs.left + 15+ r.right - r.left+5 , rs.top + 20, 100, 25, hWnd, 0, 0, 0);
		EDMazeSizeY = CreateWindow("edit", std::to_string(maze.GetHeight()).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rs.left + 15, rs.top + 50, 50, 25, hWnd, (HMENU)3, 0, 0);
		GetWindowRect(EDMazeSizeY, &r);
		CreateWindow("static", "Height", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, rs.left + r.right - r.left + 15 + 5, rs.top + 50, 100, 25, hWnd, 0, 0, 0);
		EDFieldSize = CreateWindow("edit", std::to_string(fldSize).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rs.left + 15, rs.top + 80, 50, 25, hWnd, (HMENU)3, 0, 0);
		GetWindowRect(EDMazeSizeY, &r);
		CreateWindow("static", "Tile size", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, rs.left + r.right - r.left + 15 + 5, rs.top + 80, 100, 25, hWnd, 0, 0, 0);
		hBTSetSize=CreateWindow("button", "SetSize", WS_CHILD | WS_VISIBLE | WS_BORDER, rs.left + 15, rs.top + 110, 150, 25, hWnd, (HMENU)BTSetSize, 0, 0);
		
		// Object
		GRObject = CreateWindow("button", "Objects", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_GROUPBOX, drawRect.right+3, rs.bottom+5, 180, 450, hWnd, (HMENU)-1, 0, 0);
		GetWindowRect(GRObject, &rs); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&rs, 2);
		LBObjects = CreateWindowW(L"listbox", L"d", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | WS_BORDER, rs.left+5, rs.top + 20, 170, 100, hWnd, (HMENU)0, NULL, NULL);
		for (int i = 0; i < objects.getCount(); i++) {
			SendMessage(LBObjects, LB_ADDSTRING, 0, (LPARAM)objects[i].objName);
		}
		SendMessage(LBObjects, LB_SETCURSEL, 0, 0);
		GetWindowRect(LBObjects, &r); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&r, 2);
		hBTDelete =CreateWindow("button", "Delete", WS_CHILD | WS_VISIBLE | WS_BORDER, rs.left + 15, r.bottom + 5, 150, 25, hWnd, (HMENU)BTDeleteObject, 0, 0);
		GetWindowRect(hBTDelete, &r); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&r, 2);
		htemp=CreateWindow("static", "Event Actions", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, r.left + 5, r.bottom + 5, 100, 25, hWnd, 0, 0, 0);
		GetWindowRect(htemp, &r); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&r, 2);
		for (int i = 0; i < 3; i++) {
			htemp = CreateWindow("static",(LPSTR) Actions::eventActName[i], WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, r.left , r.bottom, 100, 25, hWnd, 0, 0, 0);
			GetWindowRect(htemp, &r); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&r, 2);
			CBEventActions[i]= CreateWindow("ComboBox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, rs.left+5, r.bottom , 100, 200, hWnd, (HMENU)-2, 0, NULL);
			GetWindowRect(CBEventActions[i], &r); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&r, 2);
			EDEventChance[i] = CreateWindow("edit", "0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, r.right+5, r.top, 50, 25, hWnd, (HMENU)3, 0, 0);
			for (int j = 0; j < Actions::actCount; j++) { SendMessage(CBEventActions[i], CB_ADDSTRING, 0, (LPARAM)Actions::actName[j]); }
			SendMessage(CBEventActions[i], CB_SETCURSEL, 0, 0);
		}

		EDObjectName= CreateWindow("edit","objName", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rs.left + 5, r.bottom+5, 100, 25, hWnd, (HMENU)3, 0, 0);
		GetWindowRect(EDObjectName, &r); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&r, 2);
		htemp = CreateWindow("static", "Name", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, r.right + 5, r.top+5, 100, 25, hWnd, 0, 0, 0);
		EDObjectSize = CreateWindow("edit", "10", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rs.left + 5, r.bottom + 5, 50, 25, hWnd, (HMENU)3, 0, 0);
		GetWindowRect(EDObjectSize, &r); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&r, 2);
		htemp = CreateWindow("static", "Size", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, r.right + 5, r.top + 5, 100, 25, hWnd, 0, 0, 0);
		EDObjectSpeed = CreateWindow("edit", "3", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rs.left + 5, r.bottom + 5, 50, 25, hWnd, (HMENU)3, 0, 0);
		GetWindowRect(EDObjectSpeed, &r); MapWindowPoints(HWND_DESKTOP, (hWnd), (LPPOINT)&r, 2);
		htemp = CreateWindow("static", "Speed", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, r.right + 5, r.top + 5, 100, 25, hWnd, 0, 0, 0);
		hBTSetobjectParam = CreateWindow("button", "Set Object Parameters", WS_CHILD | WS_VISIBLE | WS_BORDER, rs.left+5, r.bottom + 5, 170, 25, hWnd, (HMENU)BTSetobjectParam, 0, 0);
		
		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	case WM_COMMAND:
		switch (HIWORD(wp)) {
		case BN_CLICKED:
			switch (LOWORD(wp)) {
			case BTSetSize:
				try {
					GetWindowText(EDMazeSizeX, buff, 49);
					intbuff[0] =std::stoi(buff);
					GetWindowText(EDMazeSizeY, buff, 49);
					intbuff[1] = std::stoi(buff);
					GetWindowText(EDFieldSize, buff, 49);
					intbuff[2] = std::stoi(buff);
					if (intbuff[1] * intbuff[0] > 10000){
						MessageBox(0, "Too big value", 0, MB_OK); break;
					}
					maze.SetSize(intbuff[0], intbuff[1]); 
					fldSize = intbuff[2];
				}
				catch (...) { MessageBox(0, "Invalid value", 0, MB_OK); }
				break;
			case BTDeleteObject:
				intbuff[0] = SendMessage(LBObjects, LB_GETCURSEL, 0, 0);
				if (intbuff[0] == -1) break;
				objects.eraze(intbuff[0]);
				SendMessage(LBObjects, LB_DELETESTRING, intbuff[0], 0);
				if (intbuff[0] < objects.getCount()) { SendMessage(LBObjects, LB_SETCURSEL, intbuff[0], 0); }
				else { 
					if (objects.getCount() == 0)break;
					SendMessage(LBObjects, LB_SETCURSEL, objects.getCount()-1 , 0); 
				}
				
				break;
			case BTSetobjectParam:
				if (intbuff[0] = SendMessage(LBObjects, LB_GETCURSEL, 0, 0) == -1)return 1;
				obj = objects[intbuff[0]];
				for (int i = 0; i < 3; i++) {
					obj.eventAct[i] = SendMessage(CBEventActions[i],CB_GETCURSEL, 0, 0);
					GetWindowText(EDEventChance[i], buff, 49);
					obj.eventActChance[i] = std::stoi(buff);
					if (obj.eventActChance[i] > 100 || obj.eventActChance[i] < 0) {
						MessageBox(0, "Invalid chance", 0, MB_OK); break;
					}
				}
				GetWindowText(EDObjectSize, buff, 49);
				obj.drawSize = std::stoi(buff);
				GetWindowText(EDObjectSpeed, buff, 49);
				obj.speed = std::stof(buff);
				GetWindowText(EDObjectName, buff, 49);
				intbuff[0] = strlen(buff) + 1;
				tempC = new char[intbuff[0]];
				for (int i = 0; i <  intbuff[0]; i++)
					tempC[i] = buff[i];
				delete[] obj.objName;
				obj.objName = tempC;
				intbuff[0] = SendMessage(LBObjects, LB_GETCURSEL, 0, 0);
				objects[intbuff[0]] = obj;
				SendMessage(LBObjects, LB_DELETESTRING, intbuff[0], 0);
				SendMessage(LBObjects, LB_INSERTSTRING, intbuff[0], (LPARAM)obj.objName);
				SendMessage(LBObjects, LB_SETCURSEL, intbuff[0], 0);
				break;
			case BTStart:
				if (mode == 0) {//EDMazeSizeX, EDMazeSizeY, EDFieldSize, GRSetsize, GRObject, EDEventChance[3], CBEventActions[3], EDObjectSize, EDObjectName, EDObjectSpeed,hBTStart,hBTSetobjectParam,hBTDelete,hBTSetSize
					EnableWindow(EDMazeSizeX, false);
					EnableWindow(EDMazeSizeY, false);
					EnableWindow(EDFieldSize, false);
					EnableWindow(GRSetsize, false);
					EnableWindow(EDObjectSize, false);
					EnableWindow(EDObjectName, false);
					EnableWindow(EDObjectSpeed, false);
					EnableWindow(hBTSetobjectParam, false);
					EnableWindow(hBTDelete, false);
					EnableWindow(hBTSetSize, false);
					EnableWindow(CBMazeTile, false);
					EnableWindow(LBObjects, false);
					
					for (int i = 0; i < 3; i++) {
						EnableWindow(EDEventChance[i], false);
						EnableWindow(CBEventActions[i], false);
					}
					mode = 1;
					SetWindowText(hBTStart, "Stop");
					updatePtr = updatePM; drawPtr = drawPM; mouseEventPtr = mouseEventPM;
					contr.setMaze(maze);
					contr.setObjects(objects);
				}
				else {
					EnableWindow(EDMazeSizeX, true);
					EnableWindow(EDMazeSizeY, true);
					EnableWindow(EDFieldSize, true);
					EnableWindow(GRSetsize, true);
					EnableWindow(EDObjectSize, true);
					EnableWindow(EDObjectName, true);
					EnableWindow(EDObjectSpeed, true);
					EnableWindow(hBTSetobjectParam, true);
					EnableWindow(hBTDelete, true);
					EnableWindow(hBTSetSize, true);
					EnableWindow(CBMazeTile, true);
					EnableWindow(LBObjects, true);

					for (int i = 0; i < 3; i++) {
						EnableWindow(EDEventChance[i], true);
						EnableWindow(CBEventActions[i], true);
					}
					mode = 0;
					SetWindowText(hBTStart, "Start");
					updatePtr = updateCM; drawPtr = drawCM; mouseEventPtr = mouseEventCM;
				}
				break;
			}
			break;
		case CBN_SELENDOK:
			switch (LOWORD(wp)) {
			case 111:
				parameters[0] = (char)SendMessage(CBMazeTile, CB_GETCURSEL, 0, 0);
				if(parameters[0] == fldType::OBJECT)mapmode = mapmodes::addObject;
					else mapmode = mapmodes::settile;
			break;
			}
			return DefWindowProc(hWnd, msg, wp, lp);
		case LBN_SELCHANGE:
			switch (LOWORD(wp)) {
			case 0 :
				obj = objects[SendMessage(LBObjects, LB_GETCURSEL, 0, 0)];
				for (int i = 0; i < 3; i++) {
					SendMessage(CBEventActions[i], CB_SETCURSEL, obj.eventAct[i], 0);
					SetWindowText(EDEventChance[i], std::to_string(obj.eventActChance[i]).c_str());
				}
				SetWindowText(EDObjectSize, std::to_string(obj.drawSize).c_str());
				SetWindowText(EDObjectSpeed, std::to_string(obj.speed).c_str());
				SetWindowText(EDObjectName, obj.objName);
				break;
			}
			break;
		}
		break;
	case WM_DESTROY:PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wp) {
		case VK_UP:
			if (maze.GetField(objects[0].GetXY(0), objects[0].GetXY(1) - 1) == fldType::EMPTY)
			objects[0].SetAction(Actions::UP);
			break;
		case VK_DOWN:
			if (maze.GetField(objects[0].GetXY(0), objects[0].GetXY(1)+1) == fldType::EMPTY)
			objects[0].SetAction(Actions::DOWN);
			break;
		case VK_LEFT:
			if (maze.GetField(objects[0].GetXY(0) - 1, objects[0].GetXY(1)) == fldType::EMPTY)
			objects[0].SetAction(Actions::LEFT);
			break;
		case VK_RIGHT:
			if (maze.GetField(objects[0].GetXY(0) + 1, objects[0].GetXY(1)) == fldType::EMPTY)
			objects[0].SetAction(Actions::RIGHT);
			break;
		}
		if (wp == VK_CONTROL) {
			maze.randomFill(ran+=245);
		}
		break;
		return DefWindowProc(hWnd, msg, wp, lp);
	case WM_MOUSEMOVE:
		s= "x: " + std::to_string((int)LOWORD(lp)) + " y: " + std::to_string((int)HIWORD(lp));
		SetWindowText(hWnd, s.c_str());
		break;
	case WM_ERASEBKGND:
		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	default:return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

LRESULT CALLBACK drawProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	std::string s;
	switch (msg) {
	case WM_CREATE:
		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	case WM_DESTROY:PostQuitMessage(0);
		break;
	case WM_PAINT:

		break;
	case WM_MOUSEMOVE:
		dmX = mX - (int)LOWORD(lp); dmY = mY - (int)HIWORD(lp);
		mX = (int)LOWORD(lp); mY = (int)HIWORD(lp);
		s = "x: " + std::to_string((int)LOWORD(lp)) + " y: " + std::to_string((int)HIWORD(lp));
		SetWindowText(MainWindow, s.c_str());
		mouseEventPtr(msg);
		break;
	case WM_LBUTTONDOWN:
		mX = (int)LOWORD(lp); mY = (int)HIWORD(lp);
		mPress = true;
		mouseEventPtr(msg);
		break;
	case WM_LBUTTONUP:
		mrX = (int)LOWORD(lp); mrY = (int)HIWORD(lp);
		mPress = false;
		mouseEventPtr(msg);
		break;
	case WM_RBUTTONDOWN:
		mrX = (int)LOWORD(lp); mrY = (int)HIWORD(lp);
		mrPress = true;
		mouseEventPtr(msg);
		break;
	case WM_RBUTTONUP:
		mrX = (int)LOWORD(lp); mrY = (int)HIWORD(lp);
		mrPress = false;
		mouseEventPtr(msg);
		break;
	case WM_MOUSELEAVE:
		mPress = false; mrPress = false;
		break;
	case WM_ERASEBKGND:
		return 1;
		break;
	default:return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}