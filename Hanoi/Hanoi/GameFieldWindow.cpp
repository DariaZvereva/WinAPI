#include "stdafx.h"
#include "GameFieldWindow.h"
#include <iostream>
#include <strsafe.h>
#include <windowsx.h>

const float MULT_TOWER_HEIGHT = 0.7;
const float MULT_DISK_WIDTH_MAX = 0.8;
const float MULT_DISK_WIDTH_MIN = 0.2;
const float MULT_TOWER_WIDTH = 0.01;

void printError(int from, int to)
{
	LPTSTR lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		0, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	// Display the error message 

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)TEXT("GetProcessId")) + 40) * sizeof(TCHAR));

	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("from  %d, to %d"),
		from, to);
		//TEXT("%s failed with error %d: %s"),
		//TEXT("GetProcessId"), dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

HWND CGameFieldWindow::GetWindowHandle() {
	return hWnd;
}
bool CGameFieldWindow::RegisterClass()
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = ClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("Call to Register child ClassEx failed!"), _T("Game Field"), NULL);
		return false;
	}
	return true;
}


bool CGameFieldWindow::Create(HWND parentWindow)
{
	hWnd = CreateWindow(
		ClassName,
		title.c_str(),
		WS_CHILD,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		parentWindow,
		NULL,
		GetModuleHandle(NULL),
		this);

	if (!hWnd) {
		MessageBox(NULL, _T("Call to Create child Window failed!"), _T("Game Field"), NULL);
		return false;
	}
	return true;
}

void CGameFieldWindow::Show(int cmdShow)
{
	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);
}


void CGameFieldWindow::OnDestroy() {
	PostQuitMessage(0);
	//KillTimer(hWnd, 1);
}

void CGameFieldWindow::OnPaint(HWND handle, PAINTSTRUCT &ps) {
	HDC hdc = BeginPaint(handle, &ps);
	RECT windRect;
	GetClientRect(handle, &windRect);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmMem, hbmOld;
	hbmMem = CreateCompatibleBitmap(hdc, windRect.right - windRect.left, windRect.bottom - windRect.top);
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

	HBRUSH hbrush, oldBrush;
	//Background
	hbrush = CreateSolidBrush(BACKGROUND_COLOR);
	oldBrush = (HBRUSH)SelectObject(hdcMem, hbrush);
	FillRect(hdcMem, &windRect, hbrush);
	SelectObject(hdcMem, oldBrush);
	DeleteObject(hbrush);
/*	HFONT font = CreateFont(
		18, //nHeight		
		0, //nWidth
		0, //nEscapement
		0, //nOrientation
		FW_NORMAL, //fnWeight
		TRUE, //fdwItalic
		FALSE, //fdwUnderline
		FALSE, //fdwStrikeOut
		DEFAULT_CHARSET, //fdwCharSet
		OUT_OUTLINE_PRECIS, //fdwOutputPrecision
		CLIP_DEFAULT_PRECIS, //fdwClipPrecision
		ANTIALIASED_QUALITY, //fdwQuality
		FF_ROMAN, //fdwPitchAndFamily
		NULL); //lpszFace;
	SetTextColor(hdcMem, TEXT_COLOR);
	SelectObject(hdcMem, font);
	RECT numStepsTextRect = { 0, 0, 50, 15 };
	RECT numStepsRect = {50, 0, 70, 15};
	DrawText(hdcMem, LPWSTR("Number of steps"), wcslen(LPWSTR("Number of steps")), &numStepsTextRect, DT_CENTER);
	DrawText(hdcMem, LPWSTR(game.getNumberOfSteps()), wcslen(LPWSTR(game.getNumberOfSteps())), &numStepsRect, DT_CENTER);
	*/
	//Towers
	drawTowers(hdcMem, windRect);
	//Disks
	drawDisks(hdcMem, windRect);

	BitBlt(hdc,
		windRect.left, windRect.top,
		windRect.right - windRect.left, windRect.bottom - windRect.top,
		hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
	EndPaint(handle, &ps);
}

void CGameFieldWindow::OnTimer() {
	
}

LRESULT CGameFieldWindow::wndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hbrush, oldBrush;
	TCHAR greeting[] = _T("Hello, World!");
	CGameFieldWindow* pThis = reinterpret_cast<CGameFieldWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	switch (message) {
	case WM_NCCREATE:
	{
		pThis = static_cast<CGameFieldWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

		//SetTimer(handle, 1, 50, NULL);
		return TRUE;
	}
	case WM_LBUTTONDOWN:
	{
		pThis->OnClick(handle, lParam);

	}
	break;
	case WM_PAINT:
	{
		pThis->OnPaint(handle, ps);
		break;
	}
	case WM_DESTROY:
	{
		pThis->OnDestroy();
		return TRUE;
	}
	}
	return DefWindowProc(handle, message, wParam, lParam);
}

void CGameFieldWindow::OnClick(HWND handle, LPARAM lParam) {
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	RECT windRect;
	GetClientRect(handle, &windRect);
	prevActDisk = activeDisk;
	prevActTower = activeTower;
	activeDisk = findActiveDisk(xPos, yPos, windRect);
	activeTower = findActiveTower(xPos, yPos, windRect);
	if ((prevActDisk >= 0) && (activeTower >= 0) && (game.isAvailableDisk(prevActDisk))) {
		GameStep step(game.getPositionOfDisk(prevActDisk).first, activeTower);
		if (game.isCorrectStep(step)) {
			game.doStep(step);
		}
		else {
			MessageBox(NULL, _T("Этот ход невозможен"), _T("Упс:("), NULL);
		}
	}
	InvalidateRect(handle, NULL, TRUE);
	if (game.isWin()) {
		MessageBox(NULL, _T("Поздравляем! Вы победили!"), _T("УРА!!!!"), NULL);
	}
}
void CGameFieldWindow::undo() {
	GameStep step = game.getPrevStep();
	step.reverse();
	game.doStep(step);
	InvalidateRect(hWnd, NULL, TRUE);
}

void CGameFieldWindow::drawTowers(HDC &hdc, RECT &windRect) {
	size_t numberOfTowers = game.getNumberOfTowers();
	int windWidth = windRect.right - windRect.left;
	int windHeight = windRect.bottom - windRect.top;
	int towerHeight = int(windHeight * MULT_TOWER_HEIGHT);
	int towerWidth = int(windWidth * MULT_TOWER_WIDTH);
	HBRUSH hbrush, oldBrush;
	for (int i = 0; i < numberOfTowers; ++i) {
		hbrush = CreateSolidBrush(TOWER_COLOR);
		oldBrush = (HBRUSH)SelectObject(hdc, hbrush);
		int left = windRect.left + (windWidth / (numberOfTowers + 1) * (i + 1) - towerWidth / 2);
		int top = windRect.top + windHeight - towerHeight;
		int right = windRect.left + (windWidth / (numberOfTowers + 1) * (i + 1) + towerWidth / 2);
		int bottom = windRect.bottom;
		RECT tower = { left, top, right, bottom };
		FillRect(hdc, &tower, hbrush);
		SelectObject(hdc, oldBrush);
		DeleteObject(hbrush);
	}
}
void CGameFieldWindow::drawDisk(size_t i, HDC &hdc, RECT &windRect, COLORREF COLOR[3]) {
	size_t numberOfTowers = game.getNumberOfTowers();
	size_t numberOfDisks = game.getNumberOfDisks();
	int windWidth = windRect.right - windRect.left;
	int windHeight = windRect.bottom - windRect.top;
	int towerHeight = int(windHeight * MULT_TOWER_HEIGHT);
	int diskHeight = int(towerHeight / numberOfDisks);
	HBRUSH hbrush, oldBrush;
	std::pair<size_t, size_t> pos = game.getPositionOfDisk(i);
	int top = windRect.top + windHeight - (pos.second + 1) * diskHeight;
	int bottom = windRect.bottom - pos.second * diskHeight;

	hbrush = CreateSolidBrush(COLOR[0]);
	oldBrush = (HBRUSH)SelectObject(hdc, hbrush);
	int diskWidth = int(windWidth / (numberOfTowers + 1) * (MULT_DISK_WIDTH_MIN + (MULT_DISK_WIDTH_MAX - MULT_DISK_WIDTH_MIN) / numberOfDisks * (numberOfDisks - i)));
	int left = windRect.left + (windWidth / (numberOfTowers + 1) * (pos.first + 1) - diskWidth / 2);
	int right = windRect.left + (windWidth / (numberOfTowers + 1) * (pos.first + 1) + diskWidth / 2);
	RECT disk = { left, top, right, bottom };
	FillRect(hdc, &disk, hbrush);

	hbrush = CreateSolidBrush(COLOR[1]);
	oldBrush = (HBRUSH)SelectObject(hdc, hbrush);
	diskWidth = int(windWidth / (numberOfTowers + 1) * (MULT_DISK_WIDTH_MIN + (MULT_DISK_WIDTH_MAX - MULT_DISK_WIDTH_MIN) / numberOfDisks * (numberOfDisks - i - 0.5)));
	left = windRect.left + (windWidth / (numberOfTowers + 1) * (pos.first + 1) - diskWidth / 2);
	right = windRect.left + (windWidth / (numberOfTowers + 1) * (pos.first + 1) + diskWidth / 2);
	disk = { left, top, right, bottom };
	FillRect(hdc, &disk, hbrush);

	hbrush = CreateSolidBrush(COLOR[2]);
	oldBrush = (HBRUSH)SelectObject(hdc, hbrush);
	diskWidth = int(windWidth / (numberOfTowers + 1) * (MULT_DISK_WIDTH_MIN + (MULT_DISK_WIDTH_MAX - MULT_DISK_WIDTH_MIN) / numberOfDisks * (numberOfDisks - i - 1)));
	left = windRect.left + (windWidth / (numberOfTowers + 1) * (pos.first + 1) - diskWidth / 2);
	right = windRect.left + (windWidth / (numberOfTowers + 1) * (pos.first + 1) + diskWidth / 2);
	disk = { left, top, right, bottom };
	FillRect(hdc, &disk, hbrush);


	//printError();
	SelectObject(hdc, oldBrush);
	DeleteObject(hbrush);
}

void CGameFieldWindow::drawDisks(HDC &hdc, RECT &windRect) {
	for (int i = 0; i < game.getNumberOfDisks(); ++i) {
		if (i == activeDisk) {
			if (game.isAvailableDisk(i)) {
				drawDisk(i, hdc, windRect, AVAILABLE_ACTIVE_DISK_COLOR);
			}
			else {
				drawDisk(i, hdc, windRect, UNAVAILABLE_ACTIVE_DISK_COLOR);
			}		  		
		}
		else {
			drawDisk(i, hdc, windRect, DISK_COLOR);
		}
	}
}

int CGameFieldWindow::findActiveDisk(int xPos, int yPos, RECT windRect) {
	size_t numberOfDisks = game.getNumberOfDisks();
	size_t numberOfTowers = game.getNumberOfTowers();
	int windWidth = windRect.right - windRect.left;
	int windHeight = windRect.bottom - windRect.top;
	int towerHeight = int(windHeight * MULT_TOWER_HEIGHT);
	int diskHeight = int(towerHeight / numberOfDisks);
	for (int i = 0; i < numberOfDisks; ++i) {
		std::pair<size_t, size_t> pos = game.getPositionOfDisk(i);
		int top = windHeight - (pos.second + 1) * diskHeight;
		int bottom = windHeight - pos.second * diskHeight;
		int diskWidth = int(windWidth / (numberOfTowers + 1) * (MULT_DISK_WIDTH_MIN + (MULT_DISK_WIDTH_MAX - MULT_DISK_WIDTH_MIN) / numberOfDisks * (numberOfDisks - i)));
		int left = (windWidth / (numberOfTowers + 1) * (pos.first + 1) - diskWidth / 2);
		int right = (windWidth / (numberOfTowers + 1) * (pos.first + 1) + diskWidth / 2);
		if ((xPos > left) && (xPos < right) && (yPos > top) && (yPos < bottom)) {
			return i;
		}
	}
	return -1;
}

int CGameFieldWindow::findActiveTower(int xPos, int yPos, RECT windRect) {
	size_t numberOfTowers = game.getNumberOfTowers();
	int windWidth = windRect.right - windRect.left;
	int windHeight = windRect.bottom - windRect.top;
	int towerWidth = int(windWidth * MULT_TOWER_WIDTH);
	int towerHeight = int(windHeight * MULT_TOWER_HEIGHT);
	int EPS = windWidth * 0.005;
	for (int i = 0; i < numberOfTowers; ++i) {
		int left = (windWidth / (numberOfTowers + 1) * (i + 1) - towerWidth / 2);
		int top = windHeight - towerHeight;
		int right = (windWidth / (numberOfTowers + 1) * (i + 1) + towerWidth / 2);
		int bottom = windHeight;
		if ((xPos > left - EPS) && (xPos < right + EPS) && (yPos > top - EPS) && (yPos < bottom)) {
			return i;
		}
	}
	return -1;
}

void CGameFieldWindow::newGame(Game &newGame) {
	game = newGame;
	InvalidateRect(hWnd, NULL, TRUE);
}