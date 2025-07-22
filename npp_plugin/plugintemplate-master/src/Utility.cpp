#include "Utility.h"
#include <Urlmon.h>
#include <fstream>      // �����ļ���ȡ
#include <sstream>   // ��Ӧ std::wostringstream
#include <vector>
#include <iomanip>   // ��Ӧ std::setw, std::hex, std::uppercase ��
#include <ShlObj.h>     // ��Ȼ������SHGetFolderPath����������������
#include <Shlwapi.h>    // ����·�������� PathRemoveFileSpecW, PathAppendW

Utility::Utility()
{

}

Utility::~Utility()
{

}

// ============================================================================
//   *** �������� 1: Notepad++ ���뵽 Windows ����ҳ��ӳ�亯�� ***
//
//   ��������ǽ����������Ĺؼ� "�����"
// ============================================================================
int Utility::mapNppEncodingToWindowsCodepage(int nppEncoding)
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
//   *** �������� 1: ͨ�ñ���ת������ ***
//   �������ַ�����Դ����(sourceCodepage)ת��ΪĿ�����(destCodepage)
// ============================================================================
std::string Utility::convertStringEncoding(const std::string& input, int sourceCodepage, int destCodepage)
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
//   *** �޸Ĳ��� 2: ȫ���ع���ȡ�������õĺ��� ***
// ============================================================================
ProxyInfo Utility::getNppUpdaterProxySettings() {
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
std::string Utility::httpGetWithProxy(const std::wstring& url, const ProxyInfo& proxy) {
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


// ============================================================================
//   *** �������� 1: ���ܱ���ת������ ***
//
//   �������UTF-8�ַ���ת��ΪĿ�����ҳ(codepage)���ַ���
//   ���Ŀ����UTF-8����ֱ�ӷ��أ��������ת����
// ============================================================================
std::string Utility::convertUtf8ToTargetEncoding(const std::string& utf8String, int targetCodepage)
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
	buffer[len] = '\0';               //�ַ�����β
	result.append(buffer);            //��ֵ
	delete[] buffer;                  //ɾ��������

	//����ֵ
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
