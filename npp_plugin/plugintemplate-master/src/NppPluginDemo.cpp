//this file is part of notepad++
//Copyright (C)2022 Don HO <don.h@free.fr>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"

extern FuncItem funcItem[nbFunc];
extern NppData nppData;


BOOL APIENTRY DllMain(HANDLE hModule, DWORD  reasonForCall, LPVOID /*lpReserved*/)
{
	try {

		switch (reasonForCall)
		{
			case DLL_PROCESS_ATTACH:
				pluginInit(hModule);
				break;

			case DLL_PROCESS_DETACH:
				pluginCleanUp();
				break;

			case DLL_THREAD_ATTACH:
				break;

			case DLL_THREAD_DETACH:
				break;
		}
	}
	catch (...) { return FALSE; }

    return TRUE;
}


extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	nppData = notpadPlusData;
	commandMenuInit();
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
	return NPP_PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	*nbF = nbFunc;
	return funcItem;
}

// 事件通知处理
extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	// 这里可以处理Notepad++的事件，例如缓冲区激活

	// 我们只关心 Scintilla (编辑器) 发送的通知
	if (notifyCode->nmhdr.hwndFrom == nppData._scintillaMainHandle ||
		notifyCode->nmhdr.hwndFrom == nppData._scintillaSecondHandle)
	{

		// 检查通知代码是否为 SCN_CONTEXTMENU
		// if (notifyCode->nmhdr.code == SCN_CONTEXTMENU)
		{
			//unsigned int code = notifyCode->nmhdr.code;

			//char szBuffer[100] = { 0 };
			//sprintf(szBuffer, "code=%d.", code);
			//::MessageBoxA(NULL, szBuffer, "", MB_OK);
			
			/*
			// 步骤 1: 获取当前编辑器的句柄
			HWND hCurrentScintilla = notifyCode->nmhdr.hwndFrom;

			// 步骤 2: 检查是否有选中的文本
			intptr_t selStart = ::SendMessage(hCurrentScintilla, SCI_GETSELECTIONSTART, 0, 0);
			intptr_t selEnd = ::SendMessage(hCurrentScintilla, SCI_GETSELECTIONEND, 0, 0);

			if (selStart != selEnd) // 如果选择的起始和结束位置不同，说明有选中的内容
			{
				// 步骤 3: 获取 Scintilla 即将显示的右键菜单的句柄
				// 这个句柄由通知的 lParam 参数传递过来
				HMENU hContextMenu = (HMENU)notifyCode->lParam;

				// 步骤 4: 为了美观，先在菜单末尾添加一个分隔符
				// MF_BYPOSITION 表示按位置插入, MF_SEPARATOR 表示分隔符
				::InsertMenu(hContextMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);

				// 步骤 5: 将我们的“翻译”命令添加到菜单末尾
				// -1 表示添加到最后
				// MF_STRING 表示这是一个文本菜单项
				// funcItem[0]._cmdID 是我们命令的ID，点击后会触发相应的函数
				// funcItem[0]._pFuncName 是我们命令的显示文本
				::InsertMenu(hContextMenu, -1, MF_BYPOSITION | MF_STRING, funcItem[0]._cmdID, funcItem[0]._pFuncName);
			}
			*/
		}
	}

	switch (notifyCode->nmhdr.code) 
	{
		case NPPN_SHUTDOWN:
		{
			commandMenuCleanUp();
		}
		break;

		default:
			return;
	}
}


// Here you can process the Npp Messages 
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// https://github.com/notepad-plus-plus/notepad-plus-plus/issues
//
extern "C" __declspec(dllexport) LRESULT messageProc(UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// 消息处理

	/*
	if (Message == WM_MOVE)
	{
		::MessageBox(NULL, "move", "", MB_OK);
	}
*/
	return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
    return TRUE;
}
#endif //UNICODE
