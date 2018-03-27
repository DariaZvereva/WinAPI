#pragma once
#include <Windows.h>
#include "Game.h"
#include "Resource.h"
#include "GameWindow.h"
#include "Hanoi.h"

class CNewGameDialog
{
public:
	CNewGameDialog(Game &game, CGameWindow &gameWnd):
		oldGame(game),
		gameWindow(gameWnd) {};
	
	INT_PTR ShowNewGameDialog(HWND windowOwner) {
		return DialogBoxParam(GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDD_HANOI_DIALOG),
			windowOwner,
			(DLGPROC)NewGameProc,
			reinterpret_cast<LPARAM>(this));
	}

private:
	Game oldGame, newGame;
	CGameWindow &gameWindow;
	HWND hSpin, hText;
	const size_t MIN_NUMBER_OF_DISKS = 1;
	const size_t MAX_NUMBER_OF_DISKS = 20;
	const size_t DEFAULT_NUMBER_OF_DISKS = 5;
	const size_t DEFAULT_NUMBER_OF_TOWERS = 3;


	static BOOL CALLBACK NewGameProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void Init(HWND newGameDialog);
	BOOL OnCommand(HWND hwndDlg, WPARAM wParam, LPARAM lParam);
	void FinishWith(Game game, HWND hwndDlg, WPARAM wParam);
	void OnTextEditorDisksNumber(HWND hwndDlg, WPARAM wParam);
	BOOL OnNotify(HWND hwndDlg, WPARAM wParam, LPARAM lParam);

};

