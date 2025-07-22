// SettingsDlg.cpp: 实现文件
//

//#include "pch.h"
#include "SettingsDlg.h"
#include "../PluginDefinition.h"


extern NppData nppData;

//CSettingsDlg::~CSettingsDlg()
//{
//}

INT_PTR CALLBACK CSettingsDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		switch (wParam)
		{
			case IDC_SAVE_BUTTON:
			{
				SaveBtnClick();
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

}


