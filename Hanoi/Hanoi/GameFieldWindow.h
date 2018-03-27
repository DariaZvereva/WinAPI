#pragma once
#include <string>
#include <Windows.h>
#include "Game.h"

class CGameFieldWindow
{
public:
	CGameFieldWindow(const std::wstring title = L"Game Field") : 
		title(title), 
		activeDisk(-1),
		activeTower(-1),
		prevActDisk(-1),
		prevActTower(-1) {};
	static bool RegisterClass();
	bool Create(HWND parentWindow);
	void Show(int cmdShow);
	HWND GetWindowHandle();
	void newGame(Game &newGame);
	void undo();
protected:
	void OnDestroy();
	void OnPaint(HWND handle, PAINTSTRUCT &ps);
	void OnTimer(); 
	void OnClick(HWND handle, LPARAM lParam);
private:
	COLORREF TEXT_COLOR = RGB(0, 0, 0);
	COLORREF TOWER_COLOR = RGB(38, 75, 222);
	COLORREF ACTIVE_TOWER_COLOR = RGB(38, 222, 93);
	COLORREF FORBIDDEN_TOWER_COLOR = RGB(222, 38, 68);

	COLORREF AVAILABLE_ACTIVE_DISK_COLOR[3] = { RGB(0, 200, 0), RGB(0, 240, 0), RGB(0, 255, 0) };
	COLORREF UNAVAILABLE_ACTIVE_DISK_COLOR[3] = { RGB(200, 0, 0), RGB(240, 0, 0), RGB(255, 0, 0) };
	COLORREF DISK_COLOR[3] = { RGB(0, 0, 200), RGB(0, 0, 240), RGB(0, 0, 255) };

	COLORREF BACKGROUND_COLOR = RGB(240, 255, 240);
	int activeDisk, activeTower, prevActDisk, prevActTower;
	Game game;
	void drawTowers(HDC &hdc, RECT &windRect);
	void drawDisks(HDC &hdc, RECT &windRect);
	void drawDisk(size_t i, HDC &hdc, RECT &windRect, COLORREF COLOR[3]);
	static constexpr wchar_t* ClassName = L"GameFieldWindow"; 
	std::wstring title; // заголовок окна
	HWND hWnd; // хэндл окна
	static LRESULT __stdcall wndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	int findActiveDisk(int xPos, int yPos, RECT windRect);
	int findActiveTower(int xPos, int yPos, RECT windRect);
};

