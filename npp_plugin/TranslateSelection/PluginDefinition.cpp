#include "framework.h"
#include "PluginDefinition.h"
#include <string>
#include <vector>
#include <Urlmon.h>
#include <Shlwapi.h>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "shlwapi.lib")

// 全局变量
NppData nppData;
FuncItem funcItem[1];

// 插件初始化
void pluginInit(const NppData& nppDataIn)
{
    nppData = nppDataIn;
    funcItem[0]._pFuncName = TEXT("Translate Selected Text");
    funcItem[0]._cmdID = IDM_TRANSLATE_SELECTION;
    funcItem[0]._pFunc = translateSelection;
    funcItem[0]._init2Check = false;
    funcItem[0]._pShKey = new ShortcutKey();
    funcItem[0]._pShKey->_isCtrl = true;
    funcItem[0]._pShKey->_isShift = true;
    funcItem[0]._pShKey->_isAlt = false;
    funcItem[0]._pShKey->_key = 'T';

    ::SendMessage(nppData._nppHandle, NPM_ADDTOOLBARICON, funcItem[0]._cmdID, NULL);
    ::SendMessage(nppData._nppHandle, NPM_SETMENUCONTAINER, 0, (LPARAM)nppData._nppHandle);
    ::SendMessage(nppData._nppHandle, NPM_ADDFUNCITEM, 0, (LPARAM)&funcItem[0]);
}

// 插件清理
void pluginCleanUp()
{
    delete funcItem[0]._pShKey;
}

// 事件通知处理
void beNotified(SCNotification* pSCNotification)
{
    // 这里可以处理Notepad++的事件，例如缓冲区激活
}

// 消息处理
LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
    return TRUE;
}

// URL编码函数
std::wstring UrlEncode(const std::wstring& str)
{
    wchar_t* encoded = new wchar_t[str.length() * 3 + 1];
    DWORD size = str.length() * 3 + 1;
    HRESULT hr = UrlEscape(str.c_str(), encoded, &size, 0);
    std::wstring result;
    if (SUCCEEDED(hr))
    {
        result = encoded;
    }
    delete[] encoded;
    return result;
}

// 从API响应中解析翻译结果 (简易版)
std::string ParseTranslation(const std::string& jsonResponse)
{
    const std::string searchKey = "\"translatedText\":\"";
    size_t startPos = jsonResponse.find(searchKey);
    if (startPos == std::string::npos) return "Translation not found.";

    startPos += searchKey.length();
    size_t endPos = jsonResponse.find("\"", startPos);
    if (endPos == std::string::npos) return "Translation format error.";

    return jsonResponse.substr(startPos, endPos - startPos);
}


// 核心翻译功能
void translateSelection()
{
    HWND hCurrentScintilla = nppData._scintillaMainHandle;

    // 1. 获取选中的文本
    intptr_t selLength = ::SendMessage(hCurrentScintilla, SCI_GETSELTEXT, 0, 0);
    if (selLength <= 1) return; // 没有选择内容或只有一个字符，不做任何事

    std::vector<char> buffer(selLength);
    ::SendMessage(hCurrentScintilla, SCI_GETSELTEXT, 0, (LPARAM)buffer.data());
    std::string selectedText(buffer.data());

    // 2. 调用免费翻译API (MyMemory API)
    std::wstring wSelectedText(selectedText.begin(), selectedText.end());
    std::wstring encodedText = UrlEncode(wSelectedText);
    std::wstring url = L"http://api.mymemory.translated.net/get?q=" + encodedText + L"&langpair=auto|en";

    IStream* pStream = nullptr;
    if (S_OK != URLOpenBlockingStream(NULL, url.c_str(), &pStream, 0, NULL))
    {
        ::MessageBoxA(nppData._nppHandle, "Failed to connect to translation service.", "Translate Plugin", MB_OK);
        return;
    }

    std::string response;
    char readBuffer[256];
    ULONG bytesRead;
    while (pStream->Read(readBuffer, sizeof(readBuffer), &bytesRead) == S_OK && bytesRead > 0)
    {
        response.append(readBuffer, bytesRead);
    }
    pStream->Release();

    // 3. 解析并获取翻译结果
    std::string translatedText = ParseTranslation(response);

    // 4. 在下一行插入翻译结果
    intptr_t currentPos = ::SendMessage(hCurrentScintilla, SCI_GETCURRENTPOS, 0, 0);
    intptr_t line = ::SendMessage(hCurrentScintilla, SCI_LINEFROMPOSITION, currentPos, 0);
    intptr_t lineEndPos = ::SendMessage(hCurrentScintilla, SCI_GETLINEENDPOSITION, line, 0);
    
    std::string textToInsert = "\r\n" + translatedText;
    ::SendMessage(hCurrentScintilla, SCI_INSERTTEXT, lineEndPos, (LPARAM)textToInsert.c_str());
}