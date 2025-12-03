#include "PhoneticFeature.h"
#include "PluginDefinition.h" // 包含 Notepad++ 和 Scintilla 的必要定义
#include "Utility.h"          // 包含工具类定义
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include <windows.h>
#include <exception> // 引入异常处理

#include "DockingFeature/SettingsDlg.h"
#include "Logger.h"

// ==========================================
// 全局变量引用 (Extern)
// 这些变量定义在主插件文件中 (例如 NppPlugin.cpp)
// ==========================================
extern NppData nppData;       // Notepad++ 的核心数据句柄
extern Utility utilityTools;  // 工具类实例
extern tTbData tbData;
extern HINSTANCE g_hInstance; // 【重要】必须引用这个，在 NppPlugin.cpp/main.cpp 中定义的

CSettingsDlg settingsDlg;

// ==========================================
// 常量定义
// ==========================================
const std::wstring DICTIONARY_API_URL = L"https://api.dictionaryapi.dev/api/v2/entries/en/";

/*
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

	Logs(L"proxy server=%s:%d", proxy.server, proxy.port);

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
*/

void translateSelection()
{
	// 1. 获取 Notepad++ 的内部编码及对应的 Windows Codepage
	LPARAM currentBufferID = ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0);
	int nppEncoding = (int)(::SendMessage(nppData._nppHandle, NPPM_GETBUFFERENCODING, (WPARAM)currentBufferID, 0));
	int windowsCodepage = utilityTools.mapNppEncodingToWindowsCodepage(nppEncoding);

	// 2. 获取选中文本
	HWND hCurrentScintilla = nppData._scintillaMainHandle;
	intptr_t selLength = ::SendMessage(hCurrentScintilla, SCI_GETSELTEXT, 0, 0);
	if (selLength <= 1) return;

	std::vector<char> buffer(selLength);
	::SendMessage(hCurrentScintilla, SCI_GETSELTEXT, 0, (LPARAM)buffer.data());
	std::string originalSelectedText(buffer.data());

	// 3. 准备 URL (将选中文本转为 UTF-8 -> URL Encode)
	std::string utf8SelectedText = utilityTools.convertStringEncoding(originalSelectedText, windowsCodepage, CP_UTF8);
	std::wstring strSelectedText = utilityTools.utf8ToWString(utf8SelectedText);
	std::wstring encodedText = utilityTools.urlEncodeW(strSelectedText);
	std::wstring url = TRANSLATE_URL + encodedText;

	// 【修改】获取代理逻辑
	// 1. 优先尝试获取插件自定义代理
	ProxyInfo proxy = utilityTools.GetPluginProxySettings();
	// 2. 如果插件没设置代理(或文件不存在)，则回退尝试 Notepad++ 的代理 (可选)
	if (!proxy.enabled) {
		// 4. 发起网络请求
		proxy = utilityTools.getNppUpdaterProxySettings();
	}

	std::string response = utilityTools.httpGetWithProxy(url, proxy);
	Logs(L"proxy server=%ws:%d", proxy.server.c_str(), proxy.port);

	// 5. 错误检查：网络错误
	if (response.empty()) {
		Logs(L"proxy server=%ws:%d", proxy.server.c_str(), proxy.port);
		::MessageBoxW(nppData._nppHandle, L"Failed to connect to translation service.", L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	Logs("res:%s", response.c_str());

	// 6. 错误检查：IP 被封禁 (Google 返回了 HTML 而不是 JSON)
	// 简单的检查方法：看响应是否以 < 开头 (HTML tag)
	size_t firstChar = response.find_first_not_of(" \t\r\n");
	if (firstChar != std::string::npos && response[firstChar] == '<') {
		::MessageBoxW(nppData._nppHandle, L"Google API returned HTML instead of JSON.\nPossible cause: IP blocked (Too many requests).", L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	// 7. 【核心修正】直接解析 UTF-8 的 response
	// 不要在这里转成 GBK，因为 JSON 本身就是 UTF-8 的
	std::string utf8TranslatedText = utilityTools.ParseGoogleTranslation(response);

	if (utf8TranslatedText.empty()) {
		// 如果解析失败，可能是格式变了，或者之前的 Parse 没找到内容
		// 可以在这里把 response 打印出来调试
		::MessageBoxW(nppData._nppHandle, L"Translation not found in response.", L"Parse Error", MB_OK | MB_ICONERROR);
		return;
	}

	// 8. 处理 JSON 中的转义字符 (可选，但推荐)
	// Google 返回的内容可能包含 \\n, \\", \\uXXXX 等。
	// 如果需要完美显示，这里应该有一个 UnescapeJSON 函数。
	// 但为了保持简单，我们先直接用。

	// 9. 将 UTF-8 的翻译结果转换回当前文档的编码
	std::string finalTranslatedText = utilityTools.convertStringEncoding(utf8TranslatedText, CP_UTF8, windowsCodepage);

	// 10. 插入文本
	std::string textToInsert = "\r\n" + finalTranslatedText;
	intptr_t currentPos = ::SendMessage(hCurrentScintilla, SCI_GETCURRENTPOS, 0, 0);
	intptr_t line = ::SendMessage(hCurrentScintilla, SCI_LINEFROMPOSITION, currentPos, 0);
	intptr_t lineEndPos = ::SendMessage(hCurrentScintilla, SCI_GETLINEENDPOSITION, line, 0);

	::SendMessage(hCurrentScintilla, SCI_INSERTTEXT, lineEndPos, (LPARAM)textToInsert.c_str());
}


// ==========================================
// 内部辅助函数：简易 JSON 字符串提取
// ==========================================

// 查找 JSON 字符串值的辅助函数
// startPos: 开始查找的位置
// key: 要查找的键 (例如 "\"definition\":\"")
// outValue: 输出找到的值
// outEndPos: 值结束的位置
// returns: 是否找到
static bool FindJsonValue(const std::string& json, size_t startPos, const std::string& key, std::string& outValue, size_t& outEndPos)
{
    if (startPos >= json.length()) return false;

    size_t keyPos = json.find(key, startPos);
    if (keyPos == std::string::npos) return false;

    size_t valueStart = keyPos + key.length();
    
    // 安全检查：防止 key 在字符串末尾
    if (valueStart >= json.length()) return false;

    size_t valueEnd = json.find("\"", valueStart);

    // 【关键修复】处理转义引号，并防止死循环或越界
    while (valueEnd != std::string::npos && valueEnd > 0 && json[valueEnd - 1] == '\\') {
        valueEnd = json.find("\"", valueEnd + 1);
    }

    // 【关键修复】如果找不到结束引号，直接返回 false，防止崩溃
    if (valueEnd == std::string::npos) return false;

    try {
        // 计算长度，确保不会下溢
        size_t len = valueEnd - valueStart;
        outValue = json.substr(valueStart, len);
        outEndPos = valueEnd;
        return true;
    }
    catch (...) {
        return false;
    }
}

// 提取音标 (保持原有逻辑)
static std::string ExtractPhonetics(const std::string& jsonContent)
{
    std::string result = "";
    std::set<std::string> uniquePhonetics;
    size_t pos = 0;
    std::string phone;
    size_t endPos;

    // 保护循环，防止死循环
    int safetyCount = 0;
    while (FindJsonValue(jsonContent, pos, "\"text\":\"", phone, endPos)) {
        pos = endPos;
        if (!phone.empty() && phone.find('/') != std::string::npos) {
            if (uniquePhonetics.find(phone) == uniquePhonetics.end()) {
                if (!result.empty()) result += "  ";
                result += phone;
                uniquePhonetics.insert(phone);
            }
        }
        if (++safetyCount > 50) break; // 强制熔断
    }

	// 后备：查找根节点的 phonetic
	if (result.empty()) {
		if (FindJsonValue(jsonContent, 0, "\"phonetic\":\"", phone, endPos)) {
			result = phone;
		}
	}
	return result;
}

// 核心解析：提取词性、定义和例句
// 格式化为：
// [noun]
//  1. 定义内容 (Ex: 例句内容)
static std::string ExtractMeanings(const std::string& jsonContent)
{
    std::stringstream ss;
    size_t currentPos = 0;
    int safetyLoop = 0;

	// 循环查找每一个 "partOfSpeech"
	while (true) {
        if (++safetyLoop > 50) break; // 防止死循环
		std::string posVal;
		size_t posEnd;
		// 查找词性
		if (!FindJsonValue(jsonContent, currentPos, "\"partOfSpeech\":\"", posVal, posEnd)) {
			break; // 没有更多词性了
		}
		currentPos = posEnd;

		ss << "\r\n[" << posVal << "]";

		// 确定当前词性块的搜索范围
		// 我们需要找到下一个 "partOfSpeech" 的位置，作为当前块的终点，防止定义串台
		size_t nextPosIdx = jsonContent.find("\"partOfSpeech\":\"", currentPos);
		if (nextPosIdx == std::string::npos) nextPosIdx = jsonContent.length();

		// 在当前词性块内查找 definitions
		size_t defSearchPos = currentPos;
		int defCount = 0;

		while (defSearchPos < nextPosIdx && defCount < 3) { // 限制每个词性最多显示3条定义
			std::string defVal;
			size_t defEnd;

			// 查找定义
			if (!FindJsonValue(jsonContent, defSearchPos, "\"definition\":\"", defVal, defEnd)) {
				break;
			}

			// 如果找到的定义超出了当前词性块范围，停止
			if (defEnd > nextPosIdx) break;

			defSearchPos = defEnd;
			defCount++;

			ss << "\r\n  " << defCount << ". " << defVal;

			// 查找紧随其后的例句 (example)
			// 必须在下一个 "definition" 之前找到
			size_t nextDefIdx = jsonContent.find("\"definition\":\"", defSearchPos);
			if (nextDefIdx == std::string::npos) nextDefIdx = nextPosIdx; // 如果没有下个定义，就限制在下个词性前

			std::string exVal;
			size_t exEnd;
			if (FindJsonValue(jsonContent, defSearchPos, "\"example\":\"", exVal, exEnd)) {
				// 确保例句属于当前定义
				if (exEnd < nextDefIdx && exEnd < nextPosIdx) {
					ss << " (Ex: " << exVal << ")";
					defSearchPos = exEnd; // 更新搜索位置
				}
			}
		}
	}
	return ss.str();
}

// ==========================================
// 主功能实现
// ==========================================
void GetWordDetailsAndInsert()
{
    // 【关键】最外层 Try-Catch，防止任何未处理异常导致 Notepad++ 崩溃
    try {
        // 1. 基础环境
        LPARAM currentBufferID = ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0);
        int nppEncoding = (int)(::SendMessage(nppData._nppHandle, NPPM_GETBUFFERENCODING, (WPARAM)currentBufferID, 0));
        int windowsCodepage = utilityTools.mapNppEncodingToWindowsCodepage(nppEncoding);

        HWND hCurrentScintilla = nppData._scintillaMainHandle;
        
        // 2. 获取文本
        intptr_t selLength = ::SendMessage(hCurrentScintilla, SCI_GETSELTEXT, 0, 0);
        if (selLength <= 1) return;

        std::vector<char> buffer(selLength);
        ::SendMessage(hCurrentScintilla, SCI_GETSELTEXT, 0, (LPARAM)buffer.data());
        std::string originalSelectedText(buffer.data());

        // 3. 清洗
        size_t first = originalSelectedText.find_first_not_of(" \t\r\n");
        size_t last = originalSelectedText.find_last_not_of(" \t\r\n");
        if (first == std::string::npos || last == std::string::npos) return;
        std::string trimmedText = originalSelectedText.substr(first, (last - first + 1));

        // 4. 网络请求
        std::string utf8SelectedText = utilityTools.convertStringEncoding(trimmedText, windowsCodepage, CP_UTF8);
        std::wstring strSelectedTextW = utilityTools.utf8ToWString(utf8SelectedText);
        std::wstring encodedText = utilityTools.urlEncodeW(strSelectedTextW);
        std::wstring url = DICTIONARY_API_URL + encodedText;

        ProxyInfo proxy = utilityTools.getNppUpdaterProxySettings();
        std::string responseJson = utilityTools.httpGetWithProxy(url, proxy);

        // 5. 结果处理
        std::string finalUtf8Text = "";

        if (responseJson.empty()) {
             finalUtf8Text = "[Error: Connection failed or empty response]";
        }
        else if (responseJson.find("No Definitions Found") != std::string::npos) {
            // 【改进】如果是复数形式没查到，提示用户
            finalUtf8Text = "[Word not found. Try searching for the singular form or root word.]";
        }
        else {
            // 正常解析
            try {
                std::string utf8Phonetics = ExtractPhonetics(responseJson);
                std::string utf8Meanings = ExtractMeanings(responseJson);

				// 6. 组装最终文本 (UTF-8)
				// 格式：
				// /phonetic/
				// [noun]
				//   1. definition (Ex: example)
                if (utf8Phonetics.empty() && utf8Meanings.empty()) {
                    // 如果解析不出任何内容（可能是 JSON 结构变了）
                    finalUtf8Text = "[Error: Could not parse dictionary data]";
                } else {
                    if (!utf8Phonetics.empty()) finalUtf8Text += utf8Phonetics;
                    finalUtf8Text += utf8Meanings;
                }
            } 
            catch (...) {
                finalUtf8Text = "[Error: Data parsing exception]";
            }
        }

		// 7. 转回文档编码并插入
		std::string finalInsertText = utilityTools.convertStringEncoding(finalUtf8Text, CP_UTF8, windowsCodepage);

		// 确保开头换行
		std::string textToInsert = "\r\n" + finalInsertText;

		intptr_t currentPos = ::SendMessage(hCurrentScintilla, SCI_GETCURRENTPOS, 0, 0);
		intptr_t line = ::SendMessage(hCurrentScintilla, SCI_LINEFROMPOSITION, currentPos, 0);
		intptr_t lineEndPos = ::SendMessage(hCurrentScintilla, SCI_GETLINEENDPOSITION, line, 0);

		::SendMessage(hCurrentScintilla, SCI_INSERTTEXT, lineEndPos, (LPARAM)textToInsert.c_str());
    }
    catch (const std::exception& e) {
        // 捕获标准异常
        ::MessageBoxA(nppData._nppHandle, e.what(), "Plugin Error", MB_OK | MB_ICONERROR);
    }
    catch (...) {
        // 捕获所有未知异常（如内存访问错误等）
        ::MessageBoxA(nppData._nppHandle, "Unknown exception occurred in GetWordDetails", "Critical Error", MB_OK | MB_ICONERROR);
    }
}
/*
void showSettingDlg()
{
	Logs("--- Start showSettingDlg ---");
	Logs("Current g_hInstance: %p", g_hInstance);
	Logs("Current Notepad++ Handle: %p", nppData._nppHandle);

	bool isAlreadyCreated = settingsDlg.isCreated();
	Logs("Is Dialog Already Created? %s", isAlreadyCreated ? "Yes" : "No");

	// 1. 检查窗口是否已经创建
	//if (!settingsDlg.isCreated())
	if (!isAlreadyCreated)
	{
		Logs("Initializing Dialog...");

		// 2. 如果未创建，先初始化 (传入 DLL 句柄和 N++ 主窗口句柄)
		// 这一步非常重要，否则无法找到 IDD_SETTINGS 资源
		settingsDlg.init(g_hInstance, nppData._nppHandle);
		Logs("Attempting to create dialog with IDD_SETTINGS value: %d", IDD_SETTINGS);

		// 3. 创建窗口
		settingsDlg.create(&tbData);
	}

	// 4. 显示并置顶
	// 判断窗口句柄是否有效，防止创建失败（例如资源ID不对）导致的崩溃
	if (settingsDlg.getHSelf()) {
		settingsDlg.display(true);
	}
	else {
		::MessageBox(nppData._nppHandle, TEXT("Failed to create dialog. Check IDD_SETTINGS in resource.h"), TEXT("Error"), MB_OK);
	}
}
*/


void showSettingDlg()
{
	Logs("--- Start showSettingDlg ---");
	Logs("Current g_hInstance: %p", g_hInstance);
	Logs("Current Notepad++ Handle: %p", nppData._nppHandle);

	// 1. 检查是否已经创建
	bool isAlreadyCreated = settingsDlg.isCreated();
	Logs("Is Dialog Already Created? %s", isAlreadyCreated ? "Yes" : "No");

	if (!isAlreadyCreated)
	{
		Logs("Initializing Dialog...");

		// Init
		settingsDlg.init(g_hInstance, nppData._nppHandle);

		// Log resource ID (假设 IDD_SETTINGS 是宏，打印它的值)
		Logs("Attempting to create dialog with IDD_SETTINGS value: %d", IDD_SETTINGS);

		// Create
		settingsDlg.create(&tbData);

		// Check result
		HWND hDlg = settingsDlg.getHSelf();
		if (hDlg == NULL)
		{
			Logs(L"ERROR: CreateDialog failed! GetLastError: %d", GetLastError());
			Logs("Possible causes: 1. Resource ID mismatch. 2. g_hInstance invalid. 3. .rc file not compiled correctly.");
			::MessageBox(nppData._nppHandle, TEXT("Failed to create dialog! Check log."), TEXT("Error"), MB_OK);
			return;
		}
		else
		{
			Logs("Dialog Created Successfully. HWND: %p", hDlg);
		}
	}

	// 2. 显示
	HWND hSelf = settingsDlg.getHSelf();
	if (hSelf)
	{
		Logs("Calculating Center Position...");

		// 加载已有配置到输入框
		ProxyInfo currentProxy = utilityTools.GetPluginProxySettings();
		if (currentProxy.enabled)
		{
			::SetDlgItemText(hSelf, IDC_EDIT_IP, currentProxy.server.c_str());
			::SetDlgItemInt(hSelf, IDC_EDIT_PORT, currentProxy.port, FALSE);
		}


		// 获取 Notepad++ 主窗口的位置
		RECT rcParent;
		::GetWindowRect(nppData._nppHandle, &rcParent);

		/*
		// 设置对话框宽高 (根据资源里的 183x173 DLU 估算，或者直接给个固定值)
		int dlgWidth = 400;  // 像素
		int dlgHeight = 350; // 像素

		// 计算居中坐标
		int x = rcParent.left + (rcParent.right - rcParent.left - dlgWidth) / 2;
		int y = rcParent.top + (rcParent.bottom - rcParent.top - dlgHeight) / 2;

		// 【关键】强制移动窗口、设置大小、置顶、并显示
		// SWP_SHOWWINDOW 标志会强制窗口显示
		::SetWindowPos(hSelf, HWND_TOP, x, y, dlgWidth, dlgHeight, SWP_SHOWWINDOW);

		Logs("SetWindowPos called with x=%d, y=%d, w=%d, h=%d", x, y, dlgWidth, dlgHeight);
		*/
		// 再次确保显示
		::ShowWindow(hSelf, SW_SHOW);
		::UpdateWindow(hSelf); // 强制重绘

		// 调用原来的 display 逻辑 (主要是为了触发 SetFocus)
		settingsDlg.display(true);

		// 最终检查
		if (IsWindowVisible(hSelf))
			Logs("SUCCESS: Window is visible now!");
		else
			Logs("ERROR: Window is STILL hidden after forcing visibility.");
	}
	else
	{
		Logs("ERROR: hSelf is NULL before display!");
	}

	Logs("--- End showSettingDlg ---");
}

