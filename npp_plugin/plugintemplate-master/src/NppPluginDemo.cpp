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

// �¼�֪ͨ����
extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	// ������Դ���Notepad++���¼������绺��������

	// ����ֻ���� Scintilla (�༭��) ���͵�֪ͨ
	if (notifyCode->nmhdr.hwndFrom == nppData._scintillaMainHandle ||
		notifyCode->nmhdr.hwndFrom == nppData._scintillaSecondHandle)
	{

		// ���֪ͨ�����Ƿ�Ϊ SCN_CONTEXTMENU
		// if (notifyCode->nmhdr.code == SCN_CONTEXTMENU)
		{
			//unsigned int code = notifyCode->nmhdr.code;

			//char szBuffer[100] = { 0 };
			//sprintf(szBuffer, "code=%d.", code);
			//::MessageBoxA(NULL, szBuffer, "", MB_OK);
			
			/*
			// ���� 1: ��ȡ��ǰ�༭���ľ��
			HWND hCurrentScintilla = notifyCode->nmhdr.hwndFrom;

			// ���� 2: ����Ƿ���ѡ�е��ı�
			intptr_t selStart = ::SendMessage(hCurrentScintilla, SCI_GETSELECTIONSTART, 0, 0);
			intptr_t selEnd = ::SendMessage(hCurrentScintilla, SCI_GETSELECTIONEND, 0, 0);

			if (selStart != selEnd) // ���ѡ�����ʼ�ͽ���λ�ò�ͬ��˵����ѡ�е�����
			{
				// ���� 3: ��ȡ Scintilla ������ʾ���Ҽ��˵��ľ��
				// ��������֪ͨ�� lParam �������ݹ���
				HMENU hContextMenu = (HMENU)notifyCode->lParam;

				// ���� 4: Ϊ�����ۣ����ڲ˵�ĩβ���һ���ָ���
				// MF_BYPOSITION ��ʾ��λ�ò���, MF_SEPARATOR ��ʾ�ָ���
				::InsertMenu(hContextMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);

				// ���� 5: �����ǵġ����롱������ӵ��˵�ĩβ
				// -1 ��ʾ��ӵ����
				// MF_STRING ��ʾ����һ���ı��˵���
				// funcItem[0]._cmdID �����������ID�������ᴥ����Ӧ�ĺ���
				// funcItem[0]._pFuncName �������������ʾ�ı�
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
	// ��Ϣ����

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
