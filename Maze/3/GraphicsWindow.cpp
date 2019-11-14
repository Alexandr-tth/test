#include "GraphicsWindow.h"


GraphicsWindow::GraphicsWindow(HINSTANCE hInst) {
//	Name = (char*)(registerNum&(~0 << 8));
	WNDCLASS wc;
	wc.hInstance = hInst;
	wc.lpszClassName = getName();
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	if (!RegisterClass(&wc))MessageBox(hWnd, "Can't register class", "Error", MB_OK);
}
char* GraphicsWindow::getName() {
	return Name;
}