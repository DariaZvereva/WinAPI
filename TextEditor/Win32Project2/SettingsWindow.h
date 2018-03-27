#pragma once
#include <windows.h>
#include <tchar.h>
#include "TextEditorWindow.h"
#include "resource1.h"
#include <Strsafe.h>

class CSettingsWindow
{
public:
	CSettingsWindow(const Settings& initialSettings, CTextEditorWindow& editorWindow) :
		previewSettings(false),
		initialSettings(initialSettings),
		modifiedSettings(initialSettings),
		editorWindow(editorWindow) {}


	void printError()
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
			TEXT("%s failed with error %d: %s"),
			TEXT("GetProcessId"), dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
	}

	INT_PTR ShowSettingsDialog(HWND windowOwner) {
		return DialogBoxParam(GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDD_DIALOG1),
			windowOwner,
			(DLGPROC)SettingsProc,
			reinterpret_cast<LPARAM>(this));
	}
private:
	static BOOL CALLBACK SettingsProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void Init(HWND settingsWindow);
	void onScroll(HWND settingsWindow, HWND scrolledItem);
	BOOL onCommand(HWND hwndDlg, WPARAM wParam, LPARAM lParam);
	void onCheckPreviewMessage(HWND hwndDlg, WPARAM wParam);
	void onButtonFontColorMessage(HWND hwndDlg, WPARAM wParam);
	void onButtonBackgroundColorMessage(HWND hwndDlg, WPARAM wParam);


	void finishWith(const Settings & settings, HWND windowHandle, WPARAM wParam);
	COLORREF chooseColor(COLORREF initialColor, HWND settingsWindow);

	bool previewSettings;
	Settings initialSettings, modifiedSettings;
	CTextEditorWindow& editorWindow;
};