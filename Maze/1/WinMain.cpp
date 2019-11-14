#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include "Maze.h"
#include <chrono>
#include "Object.h"
int WinMain(HINSTANCE hInst, HINSTANCE prev, LPTSTR cmd, int mode);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK drawProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LPSTR WinName = "MainWndClss";
Maze maze;ObjArr objects; int objectsCount = 10;
HWND MainWindow,drawW;
float x = 0, y = 0;static const int windW = 1000, windH = 800;
const RECT drawRect = {0,0,600,800};
int fldSize = 50;
int mX = 0, mY = 0, dmX = 0, dmY = 0; bool mPress = false; // mouse
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
	Rectangle(hdc,0,0, windW, windH);
	SelectObject(hdc,br);
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
	for (int i = 0; i < objects.objCount; i++) {
			EllipseM(hdc, objects[i].GetXY(0)* fldSize - objects[i].getSize()+ fldSize/2, objects[i].GetXY(1)* fldSize - objects[i].getSize() + fldSize / 2, objects[i].GetXY(0)* fldSize + fldSize/2 + objects[i].getSize(), objects[i].GetXY(1)* fldSize + fldSize/2 + objects[i].getSize());
	}
	//hOld = SelectObject(hdc, br);
	SelectObject(hdc, hOld);
	BitBlt(hdcB, 0, 0, windW, windH, hdc, 0, 0, SRCCOPY);
	EndPaint(drawW, &ps);
}
void update(float dtm) {
	x += dtm * 100; if (x > 500)x=0;
	y += dtm * 100; if (y > 600)y = 0;
	
	objects.update(maze, dtm);
	InvalidateRect(drawW, NULL, 1);
	draw();

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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	
	if (!RegisterClass(&wc))return 0;
	wc.lpszClassName = "drawWind_34526";
	wc.lpfnWndProc = drawProc;
	if (!RegisterClass(&wc))return 0;
	MainWindow = CreateWindow(WinName, "Matrix", WS_CAPTION, CW_USEDEFAULT, CW_USEDEFAULT, windW, windH, NULL, NULL, hInst, NULL);
	ShowWindow(MainWindow, mode);
	UpdateWindow(MainWindow);
	bool done = true;
	auto start = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed;
	float frameTime = 0; const float frameM = 0.02f;
	while (done) {
		elapsed = std::chrono::high_resolution_clock::now() - start;
		start = std::chrono::high_resolution_clock::now();
		if ((frameTime += elapsed.count())>frameM) {
			update(frameTime);
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
	std::string s;
	MazeObject obj;
	static int r = 0;
	switch (msg) {
	case WM_CREATE:
		drawW = CreateWindow("drawWind_34526", "add", WS_CHILD | WS_VISIBLE | WS_BORDER, drawRect.left, drawRect.top, drawRect.right, drawRect.bottom, hWnd, 0, 0, 0);
		maze.randomFill(++r);
		hdcB= GetDC(drawW);
		br = CreateSolidBrush(RGB(100, 50, 0));
		hdc = CreateCompatibleDC(hdcB);
		hMemBMP = CreateCompatibleBitmap(hdcB, windW, windH);
		SelectObject(hdc, hMemBMP);
		ReleaseDC(drawW, hdcB);
		CreateWindow("button", "add", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 25, 100, 50, hWnd, 0, 0, 0);
		CreateWindow("button", "add", WS_CHILD | WS_VISIBLE | WS_BORDER, 850, 25, 200, 50, hWnd, 0, 0, 0);
		objects[0].SetXY(0, 3); objects[0].SetXY(1, 3); objects[0].SetAction(Actions::UP);
		objects[1].SetXY(0, 5); objects[1].SetXY(1, 3);

		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	case WM_COMMAND:
		break;
	case WM_DESTROY:PostQuitMessage(0);
		break;
	case WM_PAINT:
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
			maze.randomFill(r+=245);
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
		dmX = mX - (int)LOWORD(lp);
		dmY = mY - (int)HIWORD(lp);
		mX = (int)LOWORD(lp);
		mY = (int)HIWORD(lp);
		if (mPress){
			//maze.SetField(mX / fldSize, mY / fldSize, fldType::WALL);
			camX += dmX; camY += dmY;
		}
		s = "x: " + std::to_string((int)LOWORD(lp)) + " y: " + std::to_string((int)HIWORD(lp));
		SetWindowText(MainWindow, s.c_str());
		break;
	case WM_LBUTTONDOWN:
		mX = (int)LOWORD(lp);
		mY = (int)HIWORD(lp);
		mPress = true;
		break;
	case WM_LBUTTONUP:
		mX = (int)LOWORD(lp);
		mY = (int)HIWORD(lp);
		mPress = false;
		break;
	case WM_ERASEBKGND:
		return 1;
		//return DefWindowProc(hWnd, msg, wp, lp);
		break;
	default:return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}