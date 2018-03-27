#pragma once
#include <Windows.h>
#include "resource.h" 
#include "Game.h"
#include "GameFieldWindow.h"

class CGameWindow
{
public:
	CGameWindow(const std::wstring title = L"Hanoi") : 
		title(title), 
		game() {};

	static void RegisterClass();
	void Create();
	void Show(int cmdShow) const;
	void StartGame(Game &game);

private:
	CGameFieldWindow gameField;
	Game game;
	HWND windowHandle;
	static constexpr wchar_t* ClassName = L"CHanoiGame";
	std::wstring title;
	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
	void onResize(int width, int height);
	void onOpenNewGame();
	void onDraw();
};
