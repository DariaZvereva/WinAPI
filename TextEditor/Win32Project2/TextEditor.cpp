#include <windows.h>
#include "Utils.h"
#include "resource1.h"
#include "TextEditorWindow.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

void ShowError(const std::string &message)
{
	MessageBoxA(0, message.c_str(), "Error", MB_OK);
}

void ShowError(const std::wstring &message)
{
	MessageBoxW(0, message.c_str(), L"Error", MB_OK);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	try {
		UNREFERENCED_PARAMETER(hPrevInstance);
		UNREFERENCED_PARAMETER(lpCmdLine);

		LoadStringW(hInstance, IDR_MENU1, szWindowClass, MAX_LOADSTRING);

		CTextEditorWindow textEditorWindow(L"Text Editor");

		CTextEditorWindow::RegisterClass();
		textEditorWindow.Create();
		textEditorWindow.Show(nCmdShow);


		HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MENU1));
		MSG msg;

		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return (int)msg.wParam;
	}
	catch (std::runtime_error& e) {
		ShowError(e.what());
		return 1;
	}
}