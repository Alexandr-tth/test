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
void(*updatePtr)(float); void(*drawPtr)(HDC hdc); void(*mouseEventPtr)(UINT msg);
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
	for (int i = 0; i < contr.maze.GetHeight(); i++) {
		for (int j = 0; j < contr.maze.GetWidth(); j++) {
			if (contr.maze.GetField(j, i) == fldType::EMPTY) {
			}
			else {
				RectangleM(hdc, j * fldSize, i * fldSize, j * fldSize + fldSize, i * fldSize + fldSize);
			}
		}
	}
	SetDCBrushColor(hdc, RGB(0, 0, 150)); SetDCPenColor(hdc, RGB(0, 0, 150));
	for (int i = 0; i < contr.obj.getCount(); i++) {
			EllipseM(hdc, contr.obj[i].GetXY(0)* fldSize - contr.obj[i].getSize()+ fldSize/2, contr.obj[i].GetXY(1)* fldSize - contr.obj[i].getSize() + fldSize / 2, contr.obj[i].GetXY(0)* fldSize + fldSize/2 + contr.obj[i].getSize(), contr.obj[i].GetXY(1)* fldSize + fldSize/2 + contr.obj[i].getSize());
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
				//drawRECT(hdc, j * 50, i * 50, j * 50 + 50, i * 50 + 50);
			}
		}
	}
	SelectObject(hdc, GetStockObject(WHITE_BRUSH));
	for (int i = 0; i < objects.getCount(); i++) {
		EllipseM(hdc, objects[i].GetXY(0)* fldSize - objects[i].getSize() + fldSize / 2, objects[i].GetXY(1)* fldSize - objects[i].getSize() + fldSize / 2, objects[i].GetXY(0)* fldSize + fldSize / 2 + objects[i].getSize(), objects[i].GetXY(1)* fldSize + fldSize / 2 + objects[i].getSize());
	}
	
	SetDCBrushColor(hdc, RGB(0, 255, 0));SetDCPenColor(hdc, RGB(0, 255, 0));
	MoveToEx(hdc, camOffX / fldSize * fldSize - camX, camOffY / fldSize * fldSize - camY,0);
	LineTo(hdc, camOffX / fldSize * fldSize + fldSize - camX, camOffY / fldSize * fldSize - camY);
	LineTo(hdc, camOffX / fldSize * fldSize + fldSize - camX, camOffY / fldSize * fldSize + fldSize - camY);
	LineTo(hdc, camOffX / fldSize * fldSize  - camX, camOffY / fldSize * fldSize + fldSize - camY);
	LineTo(hdc, camOffX / fldSize * fldSize - camX, camOffY / fldSize * fldSize - camY);
	MoveToEx(hdc, 0, 0, 0);
	LineTo(hdc, mX, mY);

}
void updatePM(float dtm) {
//	objects.update(maze, dtm);
	contr.update(dtm);
	InvalidateRect(drawW, NULL, 1);
}
void updateCM(float dtm) {
	InvalidateRect(drawW, NULL, 1);
}
void mouseEventCM(UINT msg) {
	switch (msg) {
	case WM_MOUSEMOVE:
		if (mapmode == settile && mPress) {
			maze.SetField(camOffX/fldSize, camOffY / fldSize, parameters[0]);
		}else if(mrPress){
			camX += dmX; camY += dmY;
		}
		break;
	case WM_LBUTTONDOWN:
		if (mapmode == settile && mPress) {
			maze.SetField(camOffX / fldSize, camOffY / fldSize, parameters[0]);
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
	updatePtr = updatePM; drawPtr = drawCM; mouseEventPtr = mouseEventCM;
	drawPtr = drawPM;
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
unsigned int count = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	RECT r;
	char buff[50]; int intbuff[2];
	static HWND EDMazeSizeX, EDMazeSizeY, gr1;
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
		//objects
		//LBObjects = CreateWindowW(L"listbox", L"d", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | WS_BORDER, drawRect.right, 100, 100, 100, hWnd, (HMENU)0, NULL, NULL);
		//maze
		CBMazeTile=CreateWindow("ComboBox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, drawRect.right, 0, 100, 200, hWnd, (HMENU)0, 0, NULL);
		for (int i = 0; i < fldType::count; i++) {
			SendMessage(CBMazeTile, CB_ADDSTRING, 0, (LPARAM)fldType::Names[i]);
		}
		GetWindowRect(CBMazeTile, &r);
		gr1 = CreateWindow("button", "maze size", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_GROUPBOX, drawRect.right, 35, 180, 130 - 30, hWnd, 0, 0, 0);

		CreateWindow("static", "Maze Tile", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, 0,0, 130, 25, gr1, 0, 0, 0);
		EDMazeSizeX = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, drawRect.right, 55, 50, 25, gr1, (HMENU)3, 0, 0);
		GetWindowRect(EDMazeSizeX, &r);
		CreateWindow("static", "Maze Size X", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, drawRect.right + r.right - r.left +5, 55, 130, 25, gr1, 0, 0, 0);
		EDMazeSizeY = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, drawRect.right, 85, 50, 25, gr1, (HMENU)3, 0, 0);
		GetWindowRect(EDMazeSizeY, &r);
		CreateWindow("static", "Maze Size Y", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, drawRect.right + r.right - r.left + 5, 85, 130, 25, gr1, 0, 0, 0);
		CreateWindow("button", "SetSize", WS_CHILD | WS_VISIBLE | WS_BORDER, drawRect.right, 105, 150, 25, gr1, (HMENU)0, 0, 0);

		objects[0].SetXY(0, 3); objects[0].SetXY(1, 3); objects[0].SetAction(Actions::UP);
		objects[1].SetXY(0, 5); objects[1].SetXY(1, 3);

		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	case WM_COMMAND:
		switch (HIWORD(wp)) {
		case BN_CLICKED:
			switch (LOWORD(wp)) {
			case 0:
				try {
					GetWindowText(EDMazeSizeX, buff, 49);
					intbuff[0] =std::stoi(buff);
					GetWindowText(EDMazeSizeY, buff, 49);
					intbuff[1] = std::stoi(buff);
					if (intbuff[1] * intbuff[0] > 10000){
						MessageBox(0, "Too big value", 0, MB_OK); break;
					}
					maze.SetSize(intbuff[0], intbuff[1]); 
				}
				catch (...) { MessageBox(0, "Invalid value", 0, MB_OK); }
				break;
			}
			break;
		case CBN_SELENDOK:
			switch (LOWORD(wp)) {
			case 0:
				parameters[0] = (char)SendMessage(CBMazeTile, CB_GETCURSEL, 0, 0); mapmode = mapmodes::settile;
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
		//return 1;
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
		//return DefWindowProc(hWnd, msg, wp, lp);
		break;
	default:return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}