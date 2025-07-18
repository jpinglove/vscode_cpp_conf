

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
#include "menuCmdID.h"
#include <Urlmon.h>
#include <Shlwapi.h>
#include <string>
#include <fstream>      // �����ļ���ȡ
//#include <sstream>      // �����ļ���
#include <sstream>   // ��Ӧ std::wostringstream
#include <ShlObj.h>     // ��Ȼ������SHGetFolderPath����������������
#include <Shlwapi.h>    // ����·�������� PathRemoveFileSpecW, PathAppendW
#include <wininet.h>    // ���� WinINET API
#include <windows.h>
#include <iomanip>   // ��Ӧ std::setw, std::hex, std::uppercase ��




#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "wininet.lib")

using namespace std;

#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define DEPRECATED(msg) __declspec(deprecated(msg))
#else
#define DEPRECATED(msg)
#endif

#define	TRANSLATE_URL	L"https://translate.googleapis.com/translate_a/single?client=gtx&sl=zh-CN&tl=en&dt=t&q="



// ������Ϣ�ṹ��
struct ProxyInfo {
	bool enabled = false;
	std::wstring server;
	INTERNET_PORT port = 0; // ��ʼֵΪ0����ʾδ����
};
// ȫ�ֱ���
//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

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
	pSk->_isShift = true;
	pSk->_isAlt = true;
	pSk->_key = 'T';

 //   setCommand(0, TEXT("Hello Notepad++"), hello, NULL, false);
	//setCommand(1, TEXT("Hello (with dialog)"), helloDlg, NULL, false);
	setCommand(0, TEXT("TranslateSelection"), translateSelection, pSk, false);
    //subclassScintillaForContextMenu(); // ��װ�Ҽ��˵�����
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

// ============================================================================
//   *** �������� 1: Notepad++ ���뵽 Windows ����ҳ��ӳ�亯�� ***
//
//   ��������ǽ����������Ĺؼ� "�����"
// ============================================================================
int mapNppEncodingToWindowsCodepage(int nppEncoding)
{
	// �ο� Notepad++ �� UniMode ö��
	switch (nppEncoding)
	{
	case 4: // UNI_UTF8 (UTF-8 BOM)
	case 5: // UNI_UTF8_NOBOM (UTF-8 without BOM)
		return CP_UTF8; // 65001

	case 0: // ANSI
		return GetACP(); // ��ȡϵͳĬ�ϴ���ҳ (���� 936)

	case 1: // UNI_UCS2_LE
		return 1200;
	case 2: // UNI_UCS2_BE
		return 1201;

	default:
		// ����δ֪�Ļ�֧�ֵı��룬���˵�ϵͳĬ�ϱ���
		return GetACP();
	}
}

std::wstring urlEncodeW(const std::wstring& wstr)
{
	// Step 1: Convert wstring (UTF-16) to UTF-8 string
	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (utf8Len <= 0) return L"";

	std::string utf8Str(utf8Len - 1, 0); // Remove null terminator
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], utf8Len, nullptr, nullptr);

	// Step 2: URL encode UTF-8 string
	std::wostringstream escaped;
	escaped.fill(L'0');
	escaped << std::hex;

	for (unsigned char c : utf8Str)
	{
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
		{
			escaped << static_cast<wchar_t>(c);
		}
		else
		{
			escaped << L'%' << std::uppercase << std::setw(2) << int(c);
		}
	}

	return escaped.str();
}

// ============================================================================
//   *** �������� 1: ͨ�ñ���ת������ ***
//
//   �������ַ�����Դ����(sourceCodepage)ת��ΪĿ�����(destCodepage)
// ============================================================================
std::string convertStringEncoding(const std::string& input, int sourceCodepage, int destCodepage)
{
	if (input.empty() || sourceCodepage == destCodepage) {
		return input;
	}

	// ����1: ��Դ�ַ���ת��Ϊ���ֽ��ַ��� (UTF-16)
	int wlen = MultiByteToWideChar(sourceCodepage, 0, input.c_str(), -1, NULL, 0);
	if (wlen == 0) return "";

	std::vector<wchar_t> wbuffer(wlen);
	MultiByteToWideChar(sourceCodepage, 0, input.c_str(), -1, wbuffer.data(), wlen);

	// ����2: �����ֽ��ַ��� (UTF-16) ת��ΪĿ������ַ���
	int mblen = WideCharToMultiByte(destCodepage, 0, wbuffer.data(), -1, NULL, 0, NULL, NULL);
	if (mblen == 0) return "";

	std::vector<char> mbbuffer(mblen);
	WideCharToMultiByte(destCodepage, 0, wbuffer.data(), -1, mbbuffer.data(), mblen, NULL, NULL);

	return std::string(mbbuffer.data());
}

// ============================================================================
//   *** �������� 1: ��XML��������ȡ��ǩֵ�ĸ������� ***
// ============================================================================
std::string extractTagValue(const std::string& content, const std::string& tagName) {
	std::string startTag = "<" + tagName + ">";
	std::string endTag = "</" + tagName + ">";

	size_t startPos = content.find(startTag);
	if (startPos == std::string::npos) return "";

	startPos += startTag.length();
	size_t endPos = content.find(endTag, startPos);
	if (endPos == std::string::npos) return "";

	return content.substr(startPos, endPos - startPos);
}


// ============================================================================
//   *** �޸Ĳ��� 2: ȫ���ع���ȡ�������õĺ��� ***
// ============================================================================
ProxyInfo getNppUpdaterProxySettings() {
	ProxyInfo proxy;
	WCHAR nppPath[MAX_PATH] = { 0 };

	// 1. ��ȡNotepad++��ִ���ļ�������·��
	// GetModuleFileNameW(NULL, ...) ��ȡ��ǰ����(notepad++.exe)��·��
	if (GetModuleFileNameW(NULL, nppPath, MAX_PATH) == 0) {
		return proxy; // ��ȡ·��ʧ�ܣ����ؿմ�����Ϣ
	}

	// 2. ������·�����Ƴ��ļ������õ�Notepad++�İ�װĿ¼
	// "C:\Program Files\Notepad++\notepad++.exe" -> "C:\Program Files\Notepad++"
	PathRemoveFileSpecW(nppPath);

	// 3. ���� gupOptions.xml ������·��
	// -> "C:\Program Files\Notepad++\updater\gupOptions.xml"
	PathAppendW(nppPath, L"updater\\gupOptions.xml");

	// 4. ��ȡ������XML�ļ�
	std::ifstream configFile(nppPath);
	if (!configFile.is_open()) {
		return proxy; // �ļ������ڻ��޷��򿪣����ؿմ�����Ϣ
	}

	std::stringstream buffer;
	buffer << configFile.rdbuf();
	std::string content = buffer.str();
	configFile.close();

	if (content.empty()) {
		return proxy; // �ļ�Ϊ�գ����ؿմ�����Ϣ
	}

	// ����������ȡ <server> �� <port> ��ֵ
	std::string serverStr = extractTagValue(content, "server");
	std::string portStr = extractTagValue(content, "port");

	// 5. ��֤����������Ϣ
	if (!serverStr.empty() && !portStr.empty()) {
		// �� server (string) ת��Ϊ wstring
		int wlen = MultiByteToWideChar(CP_UTF8, 0, serverStr.c_str(), -1, NULL, 0);
		std::vector<wchar_t> wbuffer(wlen);
		MultiByteToWideChar(CP_UTF8, 0, serverStr.c_str(), -1, wbuffer.data(), wlen);
		proxy.server = std::wstring(wbuffer.data());

		// ת���˿��ַ���Ϊ�����������д�����
		try {
			proxy.port = (INTERNET_PORT)std::stoi(portStr);
			if (proxy.port > 0 && proxy.port <= 65535) {
				proxy.enabled = true; // ֻ�е�������Ϣ����Чʱ�������ô���
			}
		}
		catch (...) {
			// �˿ںŲ���һ����Ч�����֣����ִ���Ϊ����״̬
			proxy.enabled = false;
		}
	}

	return proxy;
}

// WinINET HTTP GET ����
std::string httpGetWithProxy(const std::wstring& url, const ProxyInfo& proxy) {
	HINTERNET hInternet = NULL, hConnect = NULL;
	std::string result = "";
	DWORD bytesRead = 0;

	const wchar_t* pProxyName = NULL;
	DWORD dwInternetOpenType = INTERNET_OPEN_TYPE_DIRECT; // Ĭ��ֱ������
	std::wstring proxyStr; // �����ں����������ڣ��Ա�֤pProxyNameָ����Ч

	if (proxy.enabled && !proxy.server.empty() && proxy.port > 0) {
		dwInternetOpenType = INTERNET_OPEN_TYPE_PROXY;
		// ��ʽ������������ַ���: "http://server:port"
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

	//::MessageBoxA(nppData._nppHandle,
	//	result.c_str(),
	//	"Translate Plugin 03", MB_OK | MB_ICONERROR);

	/*
	// --- ������־д�� (�Ѹ���Ϊ�����¾ɰ汾) ---
	wchar_t configPath[MAX_PATH] = { 0 }; // ��ʼ��Ϊ���ַ���

	// ���ȳ����·��� (NPM_GETPLUGINSCONFIGDIR)
	// #ifdef ���ڱ���ʱ��飬��ʹ����ڣ�SendMessageҲ�����ھɰ�N++��ʧ��
#ifdef NPM_GETPLUGINSCONFIGDIR 
	::SendMessage(nppData._nppHandle, NPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)configPath);
#endif

	// ����·���ʧ�� (·����ȻΪ��), ��ʹ�û��˵ľɷ���
	if (configPath[0] == L'\0')
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, configPath)))
		{
			// �ֶ�ƴ��·��: %APPDATA%\Notepad++\plugins\config
			lstrcatW(configPath, L"\\Notepad++\\plugins\\config");
			// ȷ��Ŀ¼���ڣ�����������򴴽���
			CreateDirectoryW(L"C:\\Users\\<����û���>\\AppData\\Roaming\\Notepad++\\plugins", NULL);
			CreateDirectoryW(configPath, NULL);
		}
	}

	// ȷ�����ǳɹ���ȡ��·��
	if (configPath[0] != L'\0')
	{
		std::wstring logFilePath = std::wstring(configPath) + L"\\TranslateSelection_last_response.txt";
		std::ofstream logFile(logFilePath, std::ios::binary);
		if (logFile.is_open())
		{
			logFile << result;
			logFile.close();
		}
	}
	// --- ���Խ��� ---
	*/

	return result;
}


// ============================================================================
//   *** �޸Ĳ��� 1: ���½�����������Ӧ�ȸ跭�����Ӧ��ʽ ***
// ============================================================================
// �ӹȸ�API��Ӧ�н���������
// �ȸ����Ӧ��ʽ��������: [[["Hello","���",null,null,1]],null,"zh-CN",...]
// ������Ҫ��ȡ����һ��˫�����ڵ� "Hello"
std::string ParseGoogleTranslation(const std::string& jsonResponse)
{
	const std::string searchKey = "[[[\"";
	size_t startPos = jsonResponse.find(searchKey);
	if (startPos == std::string::npos) {
		return "Translation not found in response.";
	}

	startPos += searchKey.length();
	size_t endPos = jsonResponse.find("\"", startPos);
	if (endPos == std::string::npos) {
		return "Translation format error.";
	}

	return jsonResponse.substr(startPos, endPos - startPos);
}


void translateSelection()
{
	// 1. ��ȡ Notepad++ ���ڲ�����ö��ֵ
	LPARAM currentBufferID = ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0);
	int nppEncoding = (int)(::SendMessage(nppData._nppHandle, NPPM_GETBUFFERENCODING, (WPARAM)currentBufferID, 0));

	// 2. ���ؼ����� Notepad++ �ı���ӳ�䵽 Windows API ��ʶ�Ĵ���ҳ ID
	int windowsCodepage = mapNppEncodingToWindowsCodepage(nppEncoding);

	// 3. ��ȡѡ�е�ԭʼ�ı��ֽ��� (������� windowsCodepage һ��)
	HWND hCurrentScintilla = nppData._scintillaMainHandle;
	intptr_t selLength = ::SendMessage(hCurrentScintilla, SCI_GETSELTEXT, 0, 0);
	if (selLength <= 1) return;

	std::vector<char> buffer(selLength);
	::SendMessage(hCurrentScintilla, SCI_GETSELTEXT, 0, (LPARAM)buffer.data());
	std::string originalSelectedText(buffer.data());

	// 4. ��ѡ�е��ı�����ԭʼ����ͳһת��Ϊ UTF-8���Ա㷢�͸�Google
	std::string utf8SelectedText = convertStringEncoding(originalSelectedText, windowsCodepage, CP_UTF8);

	// 5. �� UTF-8 �ַ���ת��Ϊ���ֽ� (wchar_t) �Խ��� URL ����
	int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8SelectedText.c_str(), -1, NULL, 0);
	if (wlen == 0) return;
	std::vector<wchar_t> wbuffer(wlen);
	MultiByteToWideChar(CP_UTF8, 0, utf8SelectedText.c_str(), -1, wbuffer.data(), wlen);

	std::wstring encodedText = urlEncodeW(std::wstring(wbuffer.data()));

	std::wstring url = TRANSLATE_URL + encodedText;

	//::MessageBoxW(nppData._nppHandle,
	//	url.c_str(),
	//	L"Translate Plugin 66", MB_OK | MB_ICONERROR);

	// 6. ��ȡ����������������
	ProxyInfo proxy = getNppUpdaterProxySettings();

	std::string response = httpGetWithProxy(url, proxy);

	if (response.empty()) {
		::MessageBoxW(nppData._nppHandle, L"Failed to connect to translation service.\nCheck your network connection and Notepad++ proxy settings.", L"Translate Plugin Error", MB_OK | MB_ICONERROR);
		return;
	}
	//std::wstring readableResponse = gbkToWstring(response);
	//::MessageBoxW(nppData._nppHandle,
	//	readableResponse.c_str(),
	//	L"Translate Plugin 099", MB_OK | MB_ICONERROR);

	// 8. ���ؼ�����UTF-8�ķ�����ת���ص�ǰ�ĵ���ԭʼ���� (windowsCodepage)
	//std::string ftxt = convertStringEncoding(response, CP_UTF8, windowsCodepage);
	//::MessageBoxA(nppData._nppHandle,
	//	ftxt.c_str(),
	//	"Translate Plugin 03", MB_OK | MB_ICONERROR);


	// 7. ������������ (����UTF-8������ַ���)
	std::string utf8TranslatedText = ParseGoogleTranslation(response);

	// 8. ���ؼ�����UTF-8�ķ�����ת���ص�ǰ�ĵ���ԭʼ���� (windowsCodepage)
	std::string finalTranslatedText = convertStringEncoding(utf8TranslatedText, CP_UTF8, windowsCodepage);

	// 9. ׼�������������ı�
	std::string textToInsert = "\r\n" + finalTranslatedText;
	intptr_t currentPos = ::SendMessage(hCurrentScintilla, SCI_GETCURRENTPOS, 0, 0);
	intptr_t line = ::SendMessage(hCurrentScintilla, SCI_LINEFROMPOSITION, currentPos, 0);
	intptr_t lineEndPos = ::SendMessage(hCurrentScintilla, SCI_GETLINEENDPOSITION, line, 0);

	::SendMessage(hCurrentScintilla, SCI_INSERTTEXT, lineEndPos, (LPARAM)textToInsert.c_str());
}


std::wstring utf8ToWString(const std::string& str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	std::wstring wstr(len, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], len);
	return wstr;
}

std::wstring gbkToWstring(const std::string& str)
{
	int len = MultiByteToWideChar(936 /*GBK*/, 0, str.c_str(), (int)str.length(), NULL, 0);
	if (len == 0) return L"";
	std::wstring result(len, 0);
	MultiByteToWideChar(936 /*GBK*/, 0, str.c_str(), (int)str.length(), &result[0], len);
	return result;
}

/*
std::string urlEncode(const std::string& str)
{
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : str)
	{
		if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~')
		{
			escaped << c;
		}
		else
		{
			escaped << '%' << std::uppercase << std::setw(2)
				<< int((unsigned char)c) << std::nouppercase;
		}
	}

	return escaped.str();
}
*/


// ============================================================================
//   *** �������� 1: ���ܱ���ת������ ***
//
//   �������UTF-8�ַ���ת��ΪĿ�����ҳ(codepage)���ַ���
//   ���Ŀ����UTF-8����ֱ�ӷ��أ��������ת����
// ============================================================================
std::string convertUtf8ToTargetEncoding(const std::string& utf8String, int targetCodepage)
{
	// ���Ŀ������Ѿ���UTF-8 (codepage 65001)��������ת��
	if (targetCodepage == CP_UTF8 || utf8String.empty()) {
		return utf8String;
	}

	// ����1: ��Դ UTF-8 �ַ���ת��Ϊ���ֽ��ַ��� (UTF-16)
	int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, NULL, 0);
	if (wlen == 0) return "";

	std::vector<wchar_t> wbuffer(wlen);
	MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, wbuffer.data(), wlen);

	// ����2: �����ֽ��ַ��� (UTF-16) ת��ΪĿ�����ҳ���ַ���
	int mblen = WideCharToMultiByte(targetCodepage, 0, wbuffer.data(), -1, NULL, 0, NULL, NULL);
	if (mblen == 0) return "";

	std::vector<char> mbbuffer(mblen);
	WideCharToMultiByte(targetCodepage, 0, wbuffer.data(), -1, mbbuffer.data(), mblen, NULL, NULL);

	return std::string(mbbuffer.data());
}



