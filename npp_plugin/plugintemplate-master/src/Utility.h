#pragma once


#ifndef _NPP_HEADER_H_
#define _NPP_HEADER_H_

#include <string>
#include <fstream> 
using namespace std;

#endif


#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define DEPRECATED(msg) __declspec(deprecated(msg))
#else
#define DEPRECATED(msg)
#endif

#define	TRANSLATE_URL	L"https://translate.googleapis.com/translate_a/single?client=gtx&sl=zh-CN&tl=en&dt=t&q="

class Utility
{
public:
	Utility();
	~Utility();

	int mapNppEncodingToWindowsCodepage(int nppEncoding);
	std::wstring urlEncodeW(const std::wstring& wstr);
	std::string urlEncode(const std::string& str);
	std::string convertStringEncoding(const std::string& input, int sourceCodepage, int destCodepage);
	std::string extractTagValue(const std::string& content, const std::string& tagName);
	//ProxyInfo getNppUpdaterProxySettings();
	//std::string httpGetWithProxy(const std::wstring& url, const ProxyInfo& proxy);
	std::string ParseGoogleTranslation(const std::string& jsonResponse);
	std::wstring utf8ToWString(const std::string& str);
	std::wstring gbkToWstring(const std::string& str);
	std::string convertUtf8ToTargetEncoding(const std::string& utf8String, int targetCodepage);

	std::wstring Utf8ToUtf16(const std::string& utf8_string);
	std::string Utf16ToUtf8(const std::wstring& wide_string);
	std::string ConvertWStringToAnsi(std::wstring wstr);
	std::string WstringToGbk(const std::wstring& wide_string);

};

