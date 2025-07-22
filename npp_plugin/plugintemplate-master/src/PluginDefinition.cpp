

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

#ifndef _NPP_HEADER_H_
#define _NPP_HEADER_H_

#include "PluginDefinition.h"
#include "menuCmdID.h"
#include <string>
#include <windows.h>
#include "Utility.h"

using namespace std;

#endif

// 全局变量
//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];
Utility utilityTools;


//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;


//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE /*hModule*/)
{

}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{

}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );


    ShortcutKey* pSk = new ShortcutKey();
	pSk->_isCtrl = true;
	pSk->_isShift = false;
	pSk->_isAlt = true;
	pSk->_key = 'K';

 //   setCommand(0, TEXT("Hello Notepad++"), hello, NULL, false);
	setCommand(0, TEXT("TranslateSelection"), translateSelection, pSk, false);
	//setCommand(1, TEXT("Setting"), helloDlg, NULL, false);

    //subclassScintillaForContextMenu(); // 安装右键菜单钩子
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
    if (funcItem[0]._pShKey)
    	delete funcItem[0]._pShKey;
}

//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void hello()
{
    // Open a new document
    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

    // Say hello now :
    // Scintilla control has no Unicode mode, so we use (char *) here
    ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"Hello, Notepad++!");
}

void helloDlg()
{
    ::MessageBox(NULL, TEXT("Hello, Notepad++!"), TEXT("Notepad++ Plugin Template"), MB_OK);
}


void translateSelection()
{
	// 1. 获取 Notepad++ 的内部编码枚举值
	LPARAM currentBufferID = ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0);
	int nppEncoding = (int)(::SendMessage(nppData._nppHandle, NPPM_GETBUFFERENCODING, (WPARAM)currentBufferID, 0));

	// 2. 【关键】将 Notepad++ 的编码映射到 Windows API 认识的代码页 ID
	int windowsCodepage = utilityTools.mapNppEncodingToWindowsCodepage(nppEncoding);

	// 3. 获取选中的原始文本字节流 (其编码与 windowsCodepage 一致)
	HWND hCurrentScintilla = nppData._scintillaMainHandle;
	intptr_t selLength = ::SendMessage(hCurrentScintilla, SCI_GETSELTEXT, 0, 0);
	if (selLength <= 1) return;

	std::vector<char> buffer(selLength);
	::SendMessage(hCurrentScintilla, SCI_GETSELTEXT, 0, (LPARAM)buffer.data());
	std::string originalSelectedText(buffer.data());

	// 4. 将选中的文本从其原始编码统一转换为 UTF-8，以便发送给Google
	std::string utf8SelectedText = utilityTools.convertStringEncoding(originalSelectedText, windowsCodepage, CP_UTF8);

	// 5. 将 UTF-8 字符串转换为宽字节 (wchar_t) 以进行 URL 编码
	std::wstring strSelectedText = utilityTools.utf8ToWString(utf8SelectedText);

	//int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8SelectedText.c_str(), -1, NULL, 0);
	//if (wlen == 0) return;
	//std::vector<wchar_t> wbuffer(wlen);
	//MultiByteToWideChar(CP_UTF8, 0, utf8SelectedText.c_str(), -1, wbuffer.data(), wlen);
	// std::wstring(wbuffer.data())
	std::wstring encodedText = utilityTools.urlEncodeW(strSelectedText);

	std::wstring url = TRANSLATE_URL + encodedText;

	// 6. 获取代理并发起网络请求
	ProxyInfo proxy = utilityTools.getNppUpdaterProxySettings();

	std::string response = utilityTools.httpGetWithProxy(url, proxy);

	if (response.empty()) {
		::MessageBoxW(nppData._nppHandle, L"Failed to connect to translation service.\nCheck your network connection and Notepad++ proxy settings.", L"Translate Plugin Error", MB_OK | MB_ICONERROR);
		return;
	}

	// 8. 【关键】将UTF-8的翻译结果转换回当前文档的原始编码 (windowsCodepage)
	//std::wstring ftxt = utilityTools.utf8ToWString(response);
	//::MessageBoxW(nppData._nppHandle,
	//	ftxt.c_str(),
	//	L"Translate Plugin 02", MB_OK | MB_ICONERROR);

	std::wstring wstrUtf16 = utilityTools.Utf8ToUtf16(response);
	//::MessageBoxW(nppData._nppHandle,
	//	wstrUtf16.c_str(),
	//	L"Translate Plugin 02", MB_OK | MB_ICONERROR);

	std::string newResponseText = utilityTools.WstringToGbk(wstrUtf16);

	//::MessageBoxA(nppData._nppHandle,
	//	newResponseText.c_str(),
	//	"Translate Plugin 02", MB_OK | MB_ICONERROR);

	// 7. 解析出翻译结果 (这是UTF-8编码的字符串)
	std::string utf8TranslatedText = utilityTools.ParseGoogleTranslation(newResponseText);
	//::MessageBoxA(nppData._nppHandle,
	//	utf8TranslatedText.c_str(),
	//	"Translate Plugin 03", MB_OK | MB_ICONERROR);

	// 8. 【关键】将UTF-8的翻译结果转换回当前文档的原始编码 (windowsCodepage)
	std::string finalTranslatedText = utilityTools.convertStringEncoding(utf8TranslatedText, CP_UTF8, windowsCodepage);
	//::MessageBoxA(nppData._nppHandle,
	//	finalTranslatedText.c_str(),
	//	"Translate Plugin 04", MB_OK | MB_ICONERROR);

	// 9. 准备并插入最终文本
	std::string textToInsert = "\r\n" + finalTranslatedText;
	intptr_t currentPos = ::SendMessage(hCurrentScintilla, SCI_GETCURRENTPOS, 0, 0);
	intptr_t line = ::SendMessage(hCurrentScintilla, SCI_LINEFROMPOSITION, currentPos, 0);
	intptr_t lineEndPos = ::SendMessage(hCurrentScintilla, SCI_GETLINEENDPOSITION, line, 0);

	::SendMessage(hCurrentScintilla, SCI_INSERTTEXT, lineEndPos, (LPARAM)textToInsert.c_str());
}


