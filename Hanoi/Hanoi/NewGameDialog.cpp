#include "stdafx.h"
#include "NewGameDialog.h"
#include <Commctrl.h>

BOOL CALLBACK CNewGameDialog::NewGameProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	CNewGameDialog* pThis = reinterpret_cast<CNewGameDialog*>(GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
	switch (message)
	{
	case WM_INITDIALOG: {
		pThis = reinterpret_cast<CNewGameDialog*>(lParam);
		SetWindowLongPtr(hwndDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		pThis->Init(hwndDlg);
		return TRUE;
	}
	case WM_COMMAND:
		return pThis->OnCommand(hwndDlg, wParam, lParam);
	case WM_NOTIFY:
		return pThis->OnNotify(hwndDlg, wParam, lParam);
	}
	return 0;
}

void CNewGameDialog::Init(HWND newGameDialog) {
	SendDlgItemMessage(newGameDialog, IDC_SPIN_DISKS_NUMBER, UDM_SETRANGE, (WPARAM)MIN_NUMBER_OF_DISKS, (LPARAM)MAX_NUMBER_OF_DISKS);
	// Свяжем spin и text editor
	hSpin = GetDlgItem(newGameDialog, IDC_SPIN_DISKS_NUMBER);
	hText = GetDlgItem(newGameDialog, IDC_DISKS_NUMBER);
	SendMessage(hSpin, UDM_SETBUDDY, (WPARAM)hText, 0);
	SetDlgItemInt(newGameDialog, IDC_DISKS_NUMBER, DEFAULT_NUMBER_OF_DISKS, 0);
	SetDlgItemInt(newGameDialog, IDC_SPIN_DISKS_NUMBER, DEFAULT_NUMBER_OF_DISKS, 0);
}

BOOL CNewGameDialog::OnCommand(HWND hwndDlg, WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam))
	{
	case IDC_DISKS_NUMBER:
		OnTextEditorDisksNumber(hwndDlg, wParam);
		break;
	case IDOK:
		FinishWith(newGame, hwndDlg, wParam);
		return TRUE;
	case IDCANCEL:
		FinishWith(oldGame, hwndDlg, wParam);
		return TRUE;
	}
	return 0;
}

void CNewGameDialog::FinishWith(Game game, HWND hwndDlg, WPARAM wParam) {
	gameWindow.StartGame(game);
	EndDialog(hwndDlg, wParam);
}

void CNewGameDialog::OnTextEditorDisksNumber(HWND hwndDlg, WPARAM wParam) {
	switch (HIWORD(wParam)) {
	case EN_CHANGE:
		size_t disks = GetDlgItemInt(hwndDlg, IDC_DISKS_NUMBER, 0, 0);
		SetDlgItemInt(hwndDlg, IDC_SPIN_DISKS_NUMBER, disks, 0);
		newGame = Game(DEFAULT_NUMBER_OF_TOWERS, disks);
		break;
	}
}

BOOL CNewGameDialog::OnNotify(HWND hwndDlg, WPARAM wParam, LPARAM lParam) {
	switch (((LPNMHDR)lParam)->code)
	{
		case UDN_DELTAPOS:
			LPNMUPDOWN lpnmud = (LPNMUPDOWN)lParam;
			size_t disks = GetDlgItemInt(hwndDlg, IDC_SPIN_DISKS_NUMBER, 0, 0);
			if (disks + lpnmud->iDelta < MIN_NUMBER_OF_DISKS) {
				MessageBox(NULL, _T("Количество дисков должно быть положительным!"), _T("Ой, Ошибка!"), NULL);
			} else {
				if (disks + lpnmud->iDelta > MAX_NUMBER_OF_DISKS) {
					MessageBox(NULL, _T("Слишком много дисков:("), _T("Ой, Ошибка!"), NULL);
				} else {
					SetDlgItemInt(hwndDlg, IDC_DISKS_NUMBER, disks + lpnmud->iDelta, 0);
					SetDlgItemInt(hwndDlg, IDC_SPIN_DISKS_NUMBER, disks + lpnmud->iDelta, 0);
					newGame = Game(DEFAULT_NUMBER_OF_TOWERS, disks + lpnmud->iDelta);
				}
			}
			return TRUE;
	}
	return TRUE;
}