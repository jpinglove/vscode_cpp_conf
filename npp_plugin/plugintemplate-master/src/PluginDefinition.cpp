

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
#include <Urlmon.h>
#include <Shlwapi.h>
#include <string>
#include <fstream>      // 用于文件读取
//#include <sstream>      // 用于文件流
#include <sstream>   // 对应 std::wostringstream
#include <ShlObj.h>     // 虽然不再用SHGetFolderPath，但保留可能有用
#include <Shlwapi.h>    // 用于路径处理函数 PathRemoveFileSpecW, PathAppendW
#include <wininet.h>    // 用于 WinINET API
#include <windows.h>
#include <iomanip>   // 对应 std::setw, std::hex, std::uppercase 等
#include "Utility.h"
using namespace std;

#endif


#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "wininet.lib")


// 代理信息结构体
struct ProxyInfo {
	bool enabled = false;
	std::wstring server;
	INTERNET_PORT port = 0; // 初始值为0，表示未设置
};

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
// function define
//
ProxyInfo getNppUpdaterProxySettings();
std::string httpGetWithProxy(const std::wstring& url, const ProxyInfo& proxy);


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
	pSk->_key = 'T';

 //   setCommand(0, TEXT("Hello Notepad++"), hello, NULL, false);
	//setCommand(1, TEXT("Hello (with dialog)"), helloDlg, NULL, false);
	setCommand(0, TEXT("TranslateSelection"), translateSelection, pSk, false);
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
	ProxyInfo proxy = getNppUpdaterProxySettings();

	std::string response = httpGetWithProxy(url, proxy);

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



// ============================================================================
//   *** 修改部分 2: 全面重构获取代理设置的函数 ***
// ============================================================================
ProxyInfo getNppUpdaterProxySettings() {
	ProxyInfo proxy;
	WCHAR nppPath[MAX_PATH] = { 0 };

	// 1. 获取Notepad++可执行文件的完整路径
	// GetModuleFileNameW(NULL, ...) 获取当前进程(notepad++.exe)的路径
	if (GetModuleFileNameW(NULL, nppPath, MAX_PATH) == 0) {
		return proxy; // 获取路径失败，返回空代理信息
	}

	// 2. 从完整路径中移除文件名，得到Notepad++的安装目录
	// "C:\Program Files\Notepad++\notepad++.exe" -> "C:\Program Files\Notepad++"
	PathRemoveFileSpecW(nppPath);

	// 3. 构建 gupOptions.xml 的完整路径
	// -> "C:\Program Files\Notepad++\updater\gupOptions.xml"
	PathAppendW(nppPath, L"updater\\gupOptions.xml");

	// 4. 读取并解析XML文件
	std::ifstream configFile(nppPath);
	if (!configFile.is_open()) {
		return proxy; // 文件不存在或无法打开，返回空代理信息
	}

	std::stringstream buffer;
	buffer << configFile.rdbuf();
	std::string content = buffer.str();
	configFile.close();

	if (content.empty()) {
		return proxy; // 文件为空，返回空代理信息
	}

	// 从内容中提取 <server> 和 <port> 的值
	std::string serverStr = utilityTools.extractTagValue(content, "server");
	std::string portStr = utilityTools.extractTagValue(content, "port");

	// 5. 验证并填充代理信息
	if (!serverStr.empty() && !portStr.empty()) {
		// 将 server (string) 转换为 wstring
		int wlen = MultiByteToWideChar(CP_UTF8, 0, serverStr.c_str(), -1, NULL, 0);
		std::vector<wchar_t> wbuffer(wlen);
		MultiByteToWideChar(CP_UTF8, 0, serverStr.c_str(), -1, wbuffer.data(), wlen);
		proxy.server = std::wstring(wbuffer.data());

		// 转换端口字符串为整数，并进行错误处理
		try {
			proxy.port = (INTERNET_PORT)std::stoi(portStr);
			if (proxy.port > 0 && proxy.port <= 65535) {
				proxy.enabled = true; // 只有当所有信息都有效时，才启用代理
			}
		}
		catch (...) {
			// 端口号不是一个有效的数字，保持代理为禁用状态
			proxy.enabled = false;
		}
	}

	return proxy;
}

// WinINET HTTP GET 函数
std::string httpGetWithProxy(const std::wstring& url, const ProxyInfo& proxy) {
	HINTERNET hInternet = NULL, hConnect = NULL;
	std::string result = "";
	DWORD bytesRead = 0;

	const wchar_t* pProxyName = NULL;
	DWORD dwInternetOpenType = INTERNET_OPEN_TYPE_DIRECT; // 默认直接连接
	std::wstring proxyStr; // 必须在函数作用域内，以保证pProxyName指针有效

	if (proxy.enabled && !proxy.server.empty() && proxy.port > 0) {
		dwInternetOpenType = INTERNET_OPEN_TYPE_PROXY;
		// 格式化代理服务器字符串: "http://server:port"
		proxyStr = L"http://" + proxy.server + L":" + std::to_wstring(proxy.port);
		pProxyName = proxyStr.c_str();
	}

	hInternet = InternetOpenW(L"NppTranslatePlugin/1.0", dwInternetOpenType, pProxyName, NULL, 0);
	if (!hInternet) return "";


	hConnect = InternetOpenUrlW(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if (!hConnect) {
		InternetCloseHandle(hInternet);
		return "";
	}

	char buffer[4096];
	while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
		buffer[bytesRead] = '\0';
		result.append(buffer);
	}

	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
	return result;
}
