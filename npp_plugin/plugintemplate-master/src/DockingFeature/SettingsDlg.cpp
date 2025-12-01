// SettingsDlg.cpp: 实现文件
//

//#include "pch.h"
#include "SettingsDlg.h"
#include "../PluginDefinition.h"
#include "../Logger.h"

extern NppData nppData;


INT_PTR CALLBACK CSettingsDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	// =============================================================
	// 1. 处理右上角关闭按钮 (X)
	// =============================================================
	case WM_CLOSE:
	{
		Logs("WM_CLOSE received. Hiding window.");
		::ShowWindow(_hSelf, SW_HIDE);
		return TRUE; // 告诉系统我们已经处理了这个消息，不要销毁窗口
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
			case IDC_SAVE_BUTTON:
			{
				SaveBtnClick();
				return TRUE;
			}
			case IDCANCEL:
			case IDOK:
			{
				Logs("id close dialog.");
				// display(false);
				::ShowWindow(_hSelf, SW_HIDE);
				return TRUE;
			}
		}
		return FALSE;
	}

	default:
		return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

void CSettingsDlg::SaveBtnClick()
{
	/*
int line = getLine();
if (line != -1)
{
	// Get the current scintilla
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return FALSE;
	HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	::SendMessage(curScintilla, SCI_ENSUREVISIBLE, line - 1, 0);
	::SendMessage(curScintilla, SCI_GOTOLINE, line - 1, 0);
}
*/

	MessageBox(NULL, L"hello", L"hello", MB_OK);

}


