#include "Utility.h"
#include <Urlmon.h>
#include <fstream>      // 用于文件读取
#include <sstream>   // 对应 std::wostringstream
#include <vector>
#include <iomanip>   // 对应 std::setw, std::hex, std::uppercase 等
#include <ShlObj.h>     // 虽然不再用SHGetFolderPath，但保留可能有用
#include <Shlwapi.h>    // 用于路径处理函数 PathRemoveFileSpecW, PathAppendW
#include "Logger.h"

#include <shlwapi.h> // 需要 PathRemoveFileSpecW, PathAppendW
#pragma comment(lib, "shlwapi.lib") // 确保链接库

Utility::Utility()
{

}

Utility::~Utility()
{

}

// ============================================================================
//   *** 新增部分 1: Notepad++ 编码到 Windows 代码页的映射函数 ***
//
//   这个函数是解决所有问题的关键 "翻译官"
// ============================================================================
int Utility::mapNppEncodingToWindowsCodepage(int nppEncoding)
{
	// 参考 Notepad++ 的 UniMode 枚举
	switch (nppEncoding)
	{
	case 4: // UNI_UTF8 (UTF-8 BOM)
	case 5: // UNI_UTF8_NOBOM (UTF-8 without BOM)
		return CP_UTF8; // 65001

	case 0: // ANSI
		return GetACP(); // 获取系统默认代码页 (例如 936)

	case 1: // UNI_UCS2_LE
		return 1200;
	case 2: // UNI_UCS2_BE
		return 1201;

	default:
		// 对于未知的或不支持的编码，回退到系统默认编码
		return GetACP();
	}
}



std::wstring Utility::urlEncodeW(const std::wstring& wstr)
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

std::string Utility::urlEncode(const std::string& str)
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


// ============================================================================
//   *** 新增部分 1: 通用编码转换函数 ***
//   将输入字符串从源编码(sourceCodepage)转换为目标编码(destCodepage)
// ============================================================================
std::string Utility::convertStringEncoding(const std::string& input, int sourceCodepage, int destCodepage)
{
	if (input.empty() || sourceCodepage == destCodepage) {
		return input;
	}

	// 步骤1: 将源字符串转换为宽字节字符串 (UTF-16)
	int wlen = MultiByteToWideChar(sourceCodepage, 0, input.c_str(), -1, NULL, 0);
	if (wlen == 0) return "";

	std::vector<wchar_t> wbuffer(wlen);
	MultiByteToWideChar(sourceCodepage, 0, input.c_str(), -1, wbuffer.data(), wlen);

	// 步骤2: 将宽字节字符串 (UTF-16) 转换为目标编码字符串
	int mblen = WideCharToMultiByte(destCodepage, 0, wbuffer.data(), -1, NULL, 0, NULL, NULL);
	if (mblen == 0) return "";

	std::vector<char> mbbuffer(mblen);
	WideCharToMultiByte(destCodepage, 0, wbuffer.data(), -1, mbbuffer.data(), mblen, NULL, NULL);

	return std::string(mbbuffer.data());
}


// ============================================================================
//   *** 新增部分 1: 从XML内容中提取标签值的辅助函数 ***
// ============================================================================
std::string Utility::extractTagValue(const std::string& content, const std::string& tagName) {
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
//   *** 修改部分 2: 全面重构获取代理设置的函数 ***
// ============================================================================
ProxyInfo Utility::getNppUpdaterProxySettings() {
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
	std::string serverStr = extractTagValue(content, "server");
	std::string portStr = extractTagValue(content, "port");

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
std::string Utility::httpGetWithProxy(const std::wstring& url, const ProxyInfo& proxy) {
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

	//::MessageBoxA(nppData._nppHandle,
	//	result.c_str(),
	//	"Translate Plugin 03", MB_OK | MB_ICONERROR);

	/*
	// --- 调试日志写入 (已更新为兼容新旧版本) ---
	wchar_t configPath[MAX_PATH] = { 0 }; // 初始化为空字符串

	// 优先尝试新方法 (NPM_GETPLUGINSCONFIGDIR)
	// #ifdef 用于编译时检查，即使宏存在，SendMessage也可能在旧版N++上失败
#ifdef NPM_GETPLUGINSCONFIGDIR
	::SendMessage(nppData._nppHandle, NPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)configPath);
#endif

	// 如果新方法失败 (路径仍然为空), 则使用回退的旧方法
	if (configPath[0] == L'\0')
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, configPath)))
		{
			// 手动拼接路径: %APPDATA%\Notepad++\plugins\config
			lstrcatW(configPath, L"\\Notepad++\\plugins\\config");
			// 确保目录存在，如果不存在则创建它
			CreateDirectoryW(L"C:\\Users\\<你的用户名>\\AppData\\Roaming\\Notepad++\\plugins", NULL);
			CreateDirectoryW(configPath, NULL);
		}
	}

	// 确保我们成功获取了路径
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
	// --- 调试结束 ---
	*/

	return result;
}



// ============================================================================
//   *** 修改部分 1: 更新解析函数以适应谷歌翻译的响应格式 ***
// ============================================================================
// 从谷歌API响应中解析翻译结果
// 谷歌的响应格式是这样的: [[["Hello","你好",null,null,1]],null,"zh-CN",...]
// 我们需要提取出第一个双引号内的 "Hello"

/*
std::string Utility::ParseGoogleTranslation(const std::string& jsonResponse)
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
*/
// ============================================================================
//   *** 修复版: 支持转义引号的解析函数 ***
// ============================================================================
std::string Utility::ParseGoogleTranslation(const std::string& jsonResponse)
{
	// 搜索 Google JSON 的特征头
	const std::string searchKey = "[[[\"";
	size_t startPos = jsonResponse.find(searchKey);
	if (startPos == std::string::npos) {
		return ""; // 返回空表示未找到
	}

	startPos += searchKey.length();

	// 开始查找结束的引号，注意跳过转义的引号 (\")
	std::string result;
	bool escaped = false;

	// 从 startPos 开始遍历，直到找到非转义的引号
	for (size_t i = startPos; i < jsonResponse.length(); ++i) {
		char c = jsonResponse[i];

		if (escaped) {
			// 如果上一个字符是反斜杠，这个字符就被保留（即它是内容的一部分）
			result += c;
			escaped = false;
		}
		else {
			if (c == '\\') {
				// 遇到反斜杠，标记为转义状态
				// 注意：我们可能需要保留反斜杠用于后续处理，或者在这里就处理转义
				// Google 返回的通常是字面量，保留它交给后续编码转换处理比较安全
				result += c;
				escaped = true;
			}
			else if (c == '"') {
				// 遇到非转义的引号，说明字符串结束了
				return result;
			}
			else {
				result += c;
			}
		}
	}

	return ""; // 未找到闭合引号
}



// ============================================================================
//   *** 新增部分 1: 智能编码转换函数 ***
//
//   将输入的UTF-8字符串转换为目标编码页(codepage)的字符串
//   如果目标是UTF-8，则直接返回；否则进行转换。
// ============================================================================
std::string Utility::convertUtf8ToTargetEncoding(const std::string& utf8String, int targetCodepage)
{
	// 如果目标编码已经是UTF-8 (codepage 65001)，则无需转换
	if (targetCodepage == CP_UTF8 || utf8String.empty()) {
		return utf8String;
	}

	// 步骤1: 将源 UTF-8 字符串转换为宽字节字符串 (UTF-16)
	int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, NULL, 0);
	if (wlen == 0) return "";

	std::vector<wchar_t> wbuffer(wlen);
	MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, wbuffer.data(), wlen);

	// 步骤2: 将宽字节字符串 (UTF-16) 转换为目标编码页的字符串
	int mblen = WideCharToMultiByte(targetCodepage, 0, wbuffer.data(), -1, NULL, 0, NULL, NULL);
	if (mblen == 0) return "";

	std::vector<char> mbbuffer(mblen);
	WideCharToMultiByte(targetCodepage, 0, wbuffer.data(), -1, mbbuffer.data(), mblen, NULL, NULL);

	return std::string(mbbuffer.data());
}

std::wstring Utility::utf8ToWString(const std::string& str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	if (len == 0) return std::wstring(L"");

	std::wstring wstr(len, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], len);
	return wstr;
}

std::wstring Utility::gbkToWstring(const std::string& str)
{
	int len = MultiByteToWideChar(936 /*GBK*/, 0, str.c_str(), (int)str.length(), NULL, 0);
	if (len == 0) return L"";
	std::wstring result(len, 0);
	MultiByteToWideChar(936 /*GBK*/, 0, str.c_str(), (int)str.length(), &result[0], len);
	return result;
}

// Function to convert UTF-8 std::string to UTF-16 std::wstring
std::wstring Utility::Utf8ToUtf16(const std::string& utf8_string) {
	if (utf8_string.empty()) {
		return std::wstring(); // Return empty wstring if input is empty
	}

	// Determine the required buffer size for the UTF-16 string
	// Pass -1 for the input string length to indicate null-terminated
	// Pass 0 for the output buffer size to get the required size
	int wide_chars_needed = MultiByteToWideChar(
		CP_UTF8,        // Code page for UTF-8
		0,              // No flags (or MB_ERR_INVALID_CHARS for error detection)
		utf8_string.c_str(), // Input UTF-8 string
		-1,             // Input string is null-terminated
		nullptr,        // Output buffer (get size only)
		0               // Output buffer size
	);

	if (wide_chars_needed == 0) {
		// Handle error (e.g., GetLastError())
		//std::cerr << "Error determining size for UTF-8 to UTF-16 conversion: " << GetLastError() << std::endl; //
		return std::wstring();
	}

	// Allocate a buffer for the UTF-16 string
	std::wstring wide_string(wide_chars_needed, 0); // Initialize with null wide characters

	// Perform the actual conversion
	int converted_wide_chars = MultiByteToWideChar(
		CP_UTF8,
		0,
		utf8_string.c_str(),
		-1,
		&wide_string[0], // Output buffer
		wide_chars_needed // Output buffer size
	);

	if (converted_wide_chars == 0) {
		// Handle error (e.g., GetLastError())
		//std::cerr << "Error performing UTF-8 to UTF-16 conversion: " << GetLastError() << std::endl; //
		return std::wstring();
	}

	// Resize to remove any potential null terminator if it was added implicitly
	wide_string.resize(converted_wide_chars - 1); // Subtract 1 for the null terminator MultiByteToWideChar includes

	return wide_string;
}


// Function to convert UTF-16 std::wstring to UTF-8 std::string
std::string Utility::Utf16ToUtf8(const std::wstring& wide_string) {
	if (wide_string.empty()) {
		return std::string(); // Return empty string if input is empty
	}

	// Determine the required buffer size for the UTF-8 string
	int utf8_bytes_needed = WideCharToMultiByte(
		CP_UTF8,    // Code page for UTF-8
		0,          // No flags
		wide_string.c_str(), // Input UTF-16 string
		-1,         // Input string is null-terminated
		nullptr,    // Output buffer (get size only)
		0,          // Output buffer size
		nullptr,    // Default char (not used with CP_UTF8)
		nullptr     // Used default char flag (not used with CP_UTF8)
	);

	if (utf8_bytes_needed == 0) {
		// Handle error (e.g., GetLastError())
		//std::cerr << "Error determining size for UTF-16 to UTF-8 conversion: " << GetLastError() << std::endl; //
		return std::string();
	}

	// Allocate a buffer for the UTF-8 string
	std::string utf8_string(utf8_bytes_needed, 0); // Initialize with null characters

	// Perform the actual conversion
	int converted_utf8_bytes = WideCharToMultiByte(
		CP_UTF8,
		0,
		wide_string.c_str(),
		-1,
		&utf8_string[0], // Output buffer
		utf8_bytes_needed, // Output buffer size
		nullptr,
		nullptr
	);

	if (converted_utf8_bytes == 0) {
		// Handle error (e.g., GetLastError())
		//std::cerr << "Error performing UTF-16 to UTF-8 conversion: " << GetLastError() << std::endl; //
		return std::string();
	}

	// Resize to remove any potential null terminator if it was added implicitly
	utf8_string.resize(converted_utf8_bytes - 1); // Subtract 1 for the null terminator WideCharToMultiByte includes

	return utf8_string;
}

std::string Utility::ConvertWStringToAnsi(std::wstring wstr)
{
	std::string result;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	if (len <= 0)
		return result;

	char* buffer = new char[len + 1];
	if (buffer == NULL)
		return result;

	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';               //字符串断尾
	result.append(buffer);            //赋值
	delete[] buffer;                  //删除缓冲区

	//返回值
	return result;
}

// Function to convert std::wstring (UTF-16) to std::string (GBK)
std::string Utility::WstringToGbk(const std::wstring& wide_string) {
	if (wide_string.empty()) {
		return std::string(); // Return empty string if input is empty
	}

	// Determine the required buffer size for the GBK string
	// Pass 936 for the GBK code page
	// Pass 0 for the output buffer size to get the required size, {Link: notes WideCharToMultiByte function (stringapiset.h) - Win32 apps https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte}
	int gbk_bytes_needed = WideCharToMultiByte(
		936,        // Code page for GBK (Simplified Chinese)
		0,          // No flags
		wide_string.c_str(), // Input UTF-16 string
		-1,         // Input string is null-terminated
		nullptr,    // Output buffer (get size only)
		0,          // Output buffer size
		nullptr,    // Default char (not used with GBK as it's a CJK code page)
		nullptr     // Used default char flag (not used with GBK)
	);

	if (gbk_bytes_needed == 0) {
		// Handle error (e.g., GetLastError())
		//std::cerr << "Error determining size for UTF-16 to GBK conversion: " << GetLastError() << std::endl;
		return std::string();
	}

	// Allocate a buffer for the GBK string
	std::string gbk_string(gbk_bytes_needed, 0); // Initialize with null characters

	// Perform the actual conversion
	int converted_gbk_bytes = WideCharToMultiByte(
		936,
		0,
		wide_string.c_str(),
		-1,
		&gbk_string[0], // Output buffer (use &gbk_string[0] for std::string)
		gbk_bytes_needed, // Output buffer size
		nullptr,
		nullptr
	);

	if (converted_gbk_bytes == 0) {
		// Handle error (e.g., GetLastError())
		//std::cerr << "Error performing UTF-16 to GBK conversion: " << GetLastError() << std::endl;
		return std::string();
	}

	// Resize to remove any potential null terminator if it was added implicitly
	gbk_string.resize(converted_gbk_bytes - 1); // Subtract 1 for the null terminator WideCharToMultiByte includes

	return gbk_string;
}

// ----------------------------------------------------------------------------
// 私有辅助：获取 setting.xml 的绝对路径 (位于插件DLL同级目录)
// ----------------------------------------------------------------------------
std::wstring Utility::GetSettingsFilePath()
{
	HMODULE hModule = NULL;

	// 【修改点 1】定义一个静态变量
	// 静态变量存储在 DLL 的数据段中，其地址一定属于当前 DLL 模块范围
	static int dummyMarker = 0;

	// 【修改点 2】传入静态变量的地址，并强制转换为 LPCWSTR
	if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCWSTR)&dummyMarker, // 这里改用了 dummyMarker 的地址
		&hModule))
	{
		WCHAR path[MAX_PATH];
		if (GetModuleFileNameW(hModule, path, MAX_PATH) > 0)
		{
			// 移除文件名 (TranslateSelection.dll)，只保留目录
			PathRemoveFileSpecW(path);
			// 拼接配置文件名
			PathAppendW(path, L"setting.xml");
			return std::wstring(path);
		}
	}
	return L"";
}
//std::wstring Utility::GetSettingsFilePath()
//{
//	HMODULE hModule = NULL;
//	// 获取当前 Utility 代码所在的 DLL 模块句柄
//	if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
//		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
//		(LPCTSTR)&Utility::GetSettingsFilePath, &hModule))
//	{
//		WCHAR path[MAX_PATH];
//		if (GetModuleFileNameW(hModule, path, MAX_PATH) > 0)
//		{
//			// 移除文件名 (TranslateSelection.dll)，只保留目录
//			PathRemoveFileSpecW(path);
//			// 拼接配置文件名
//			PathAppendW(path, L"setting.xml");
//			return std::wstring(path);
//		}
//	}
//	return L"";
//}


// ----------------------------------------------------------------------------
// 【功能 1】保存代理设置
// 参数：宽字符的 IP 和 端口字符串 (通常直接来自 Edit 控件)
// ----------------------------------------------------------------------------
bool Utility::SaveProxySettings(const std::wstring& server, const std::wstring& portStr)
{
	std::wstring filePath = GetSettingsFilePath();
	if (filePath.empty()) return false;

	// 1. 简单校验
	if (server.empty() || portStr.empty()) {
		return false;
	}

	// 2. 转换为 UTF-8 (为了保存到 XML)
	// 利用你现有的 Utf16ToUtf8 函数
	std::string serverUtf8 = Utf16ToUtf8(server);
	std::string portUtf8 = Utf16ToUtf8(portStr);

	// 3. 构造 XML 内容
	std::stringstream ss;
	ss << "<Settings><Proxy><server>" << serverUtf8
		<< "</server><port>" << portUtf8
		<< "</port></Proxy></Settings>";
	//std::stringstream ss;
	//ss << "<Settings>" << std::endl;
	//ss << "    <Proxy>" << std::endl;
	//ss << "        <server>" << serverUtf8 << "</server>" << std::endl;
	//ss << "        <port>" << portUtf8 << "</port>" << std::endl;
	//ss << "    </Proxy>" << std::endl;
	//ss << "</Settings>";

	// 4. 写入文件
	std::ofstream outFile(filePath); // 默认 ASCII/UTF-8 写入
	if (outFile.is_open()) {
		outFile << ss.str();
		outFile.close();
		return true;
	}

	return false;
}


// ----------------------------------------------------------------------------
// 【功能 2】读取自定义代理设置
// ----------------------------------------------------------------------------
ProxyInfo Utility::GetPluginProxySettings()
{
	ProxyInfo proxy;
	proxy.enabled = false;

	std::wstring filePath = GetSettingsFilePath();
	if (filePath.empty()) return proxy;

	// 1. 读取文件内容
	std::ifstream inFile(filePath);
	if (!inFile.is_open()) return proxy;

	std::stringstream buffer;
	buffer << inFile.rdbuf();
	std::string content = buffer.str();
	inFile.close();

	if (content.empty()) return proxy;

	// 2. 解析 XML
	// 先提取 <Proxy> 块，防止匹配到其他无关内容
	std::string proxyBlock = extractTagValue(content, "Proxy");
	if (proxyBlock.empty()) return proxy;

	// 在 Proxy 块内提取 server 和 port
	std::string serverUtf8 = extractTagValue(proxyBlock, "server");
	std::string portUtf8 = extractTagValue(proxyBlock, "port");

	// 3. 填充结构体
	if (!serverUtf8.empty() && !portUtf8.empty()) {
		// UTF-8 string -> wstring (server)
		proxy.server = Utf8ToUtf16(serverUtf8);

		// string -> int (port)
		try {
			int port = std::stoi(portUtf8);
			if (port > 0 && port <= 65535) {
				proxy.port = (INTERNET_PORT)port;
				proxy.enabled = true; // 只有解析成功才启用
			}
		}
		catch (...) {
			proxy.enabled = false;
		}
	}

	return proxy;
}

