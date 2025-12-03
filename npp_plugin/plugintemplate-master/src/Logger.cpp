#include "Logger.h"
#include <fstream>
#include <vector>
#include <cstdarg>
#include <cstdio>
#include <iostream>

BOOL Logger::bIsEnable = TRUE;

// 获取当前时间头
std::string Logger::GetTimeHeader()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	char buffer[64];
	// 格式: [2025/11/29-16:55:31+845]
	sprintf_s(buffer, "[%04d/%02d/%02d-%02d:%02d:%02d+%03d] ",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return std::string(buffer);
}

// 获取日志路径 (DLL同级目录)
std::wstring Logger::GetLogFilePath()
{
	static std::wstring logPath;
	if (logPath.empty())
	{
		HMODULE hModule = NULL;
		// 获取当前函数所在的模块(DLL)句柄，而不是 Notepad++.exe 的句柄
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCTSTR)&GetLogFilePath, &hModule);

		WCHAR path[MAX_PATH];
		if (GetModuleFileNameW(hModule, path, MAX_PATH) > 0)
		{
			std::wstring fullPath(path);
			// 去掉文件名，只保留目录
			size_t pos = fullPath.find_last_of(L"\\/");
			if (pos != std::string::npos)
			{
				logPath = fullPath.substr(0, pos + 1);
			}
			logPath += L"Debug_Plugin.log";
		}
		else
		{
			logPath = L"C:\\Temp\\Debug_Plugin.log"; // 备用路径
		}
	}
	return logPath;
}

// 窄字符实现
void Logger::WriteToFile(const char* format, ...)
{
	if (!bIsEnable) return;
	// 格式化消息
	va_list args;
	va_start(args, format);
	int len = _vscprintf(format, args) + 1;
	std::vector<char> buffer(len);
	vsnprintf(buffer.data(), len, format, args);
	va_end(args);

	std::string timeStr = GetTimeHeader();

	// 写入文件 (追加模式)
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, GetLogFilePath().c_str(), L"a+");
	if (err == 0 && fp)
	{
		fprintf(fp, "%s%s\n", timeStr.c_str(), buffer.data());
		fclose(fp);
	}
}

// 宽字符实现
void Logger::WriteToFile(const wchar_t* format, ...)
{
	if (!bIsEnable) return;

	// 格式化消息
	va_list args;
	va_start(args, format);
	int len = _vscwprintf(format, args) + 1;
	std::vector<wchar_t> buffer(len);
	vswprintf(buffer.data(), len, format, args);
	va_end(args);

	std::string timeStr = GetTimeHeader();

	// 写入文件 (追加模式)
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, GetLogFilePath().c_str(), L"a+, ccs=UTF-8");
	if (err == 0 && fp)
	{
		// 宽字符写入略有不同，先写时间(ANSI)，再写内容
		// 为了简单起见，我们把时间头转为宽字符一起写，或者分开写
		fwprintf(fp, L"%S%s\n", timeStr.c_str(), buffer.data());
		fclose(fp);
	}
}


