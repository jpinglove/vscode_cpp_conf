#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <windows.h>
#include <string>

class Logger
{
public:
	// 支持窄字符 (char*)
	static void WriteToFile(const char* format, ...);

	// 支持宽字符 (wchar_t*)
	static void WriteToFile(const wchar_t* format, ...);

private:
	static BOOL bIsEnable;
	// 获取日志文件的完整路径 (与 DLL 同目录)
	static std::wstring GetLogFilePath();

	// 获取当前时间字符串 [YYYY/MM/DD-HH:MM:SS+MS]
	static std::string GetTimeHeader();
};

// =========================================================
// 新增：简化调用的宏定义
// 使用方法: 
//   Logs("Value is %d", 100); 
//   Logs(L"宽字符 Value is %d", 100);
// =========================================================
#define Logs(...) Logger::WriteToFile(__VA_ARGS__)
#define LogTrace(fmt, ...) \
    Logger::WriteToFile("[%s:%d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#endif // _LOGGER_H_


