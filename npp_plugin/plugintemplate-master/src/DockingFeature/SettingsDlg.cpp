// SettingsDlg.cpp: 实现文件
//

//#include "pch.h"
#include "../Utility.h"
#include "SettingsDlg.h"
#include "../PluginDefinition.h"
#include "../Logger.h"

extern Utility utilityTools; // 引用全局工具对象
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

	// 1. 获取 Edit 控件内容
	const int ilen = MAX_PATH >> 2;
	WCHAR szIP[MAX_PATH] = { 0 };
	WCHAR szPort[ilen] = { 0 };

	::GetDlgItemText(_hSelf, IDC_EDIT_IP, szIP, MAX_PATH);
	::GetDlgItemText(_hSelf, IDC_EDIT_PORT, szPort, ilen);

	// 2. 调用 Utility 保存
	if (utilityTools.SaveProxySettings(szIP, szPort))
	{
		::MessageBox(_hSelf, TEXT("Settings Saved Successfully!"), TEXT("Info"), MB_OK | MB_ICONINFORMATION);
		// 保存成功后隐藏窗口
		::ShowWindow(_hSelf, SW_HIDE);
	}
	else
	{
		::MessageBox(_hSelf, TEXT("Failed to save settings.\nPlease check input."), TEXT("Error"), MB_OK | MB_ICONERROR);
	}

}


