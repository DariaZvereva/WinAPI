#include "stdafx.h"
#include "GameWindow.h"
#include "NewGameDialog.h"
#include "Game.h"
#include <string>


void CGameWindow::RegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_HANOI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_HANOI);
	wcex.lpszClassName = ClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (RegisterClassExW(&wcex) == 0) {
		throw std::runtime_error("Fail to RegisterClass " + std::to_string(GetLastError()));
	}
}


void CGameWindow::Create()
{
	windowHandle = CreateWindow(
		ClassName,  
		title.c_str(),  
		WS_OVERLAPPEDWINDOW,  
		CW_USEDEFAULT,  
		CW_USEDEFAULT,  
		CW_USEDEFAULT,  
		CW_USEDEFAULT,  
		static_cast<HWND>(NULL),  
		static_cast<HMENU>(NULL),  
		GetModuleHandle(NULL),  
		this);

	if (windowHandle == NULL) {
		throw std::runtime_error("Fail to CreateWindow " + std::to_string(GetLastError()));
	}
	SetWindowText(windowHandle, title.c_str());
	CGameFieldWindow::RegisterClass();
	gameField.Create(windowHandle);
	gameField.Show(SW_SHOW);
}


void CGameWindow::Show(int cmdShow) const {
	ShowWindow(windowHandle, cmdShow);
	UpdateWindow(windowHandle);
}

LRESULT CGameWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CGameWindow* pThis = reinterpret_cast<CGameWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	switch (message)
	{
	case WM_NCCREATE: {
		pThis = static_cast<CGameWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		return TRUE;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_FILE_NEWGAME:
			pThis->onOpenNewGame();
			break;
		case ID_FILE_UNDO:
			pThis->gameField.undo();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_SIZE:
		pThis->onResize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void CGameWindow::onResize(int width, int height) {
	RECT parentRect;
	GetClientRect(windowHandle, &parentRect);
	MoveWindow(gameField.GetWindowHandle(), 0, 0, width, height, TRUE);
}

void CGameWindow::onOpenNewGame() {
	CNewGameDialog newGameWindow(game, *this);
	newGameWindow.ShowNewGameDialog(windowHandle);
}

void CGameWindow::StartGame(Game& newGame) {
	this->game = newGame;
//	MessageBox(NULL, _T("Çàïóñòèëè èãðó"), _T("..."), NULL);
	gameField.newGame(newGame);

	//InvalidateRect(windowHandle, NULL, TRUE);
}

void CGameWindow::onDraw() {
	MessageBox(NULL, _T("ÒÓÒ ÂÑ¨ ÊÐÀÑÈÂÎ ÍÀÐÈÑÎÂÀÍÎ"), _T("..."), NULL);
}