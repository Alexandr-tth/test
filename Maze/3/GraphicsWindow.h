#pragma once

#include <Windows.h>
#include <string.h>
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
class GraphicsWindow {
	int windW = 1000, windH = 800;
	GraphicsWindow(HINSTANCE);
	~GraphicsWindow();
	static size_t registerNum;
	char* Name;
	HWND hWnd;
	char* getName();
	
	
private:

};
