#include "pch.h"
#include "Log.h"

namespace Soul {
	std::queue<std::wstring> Log::m_Logs;

	void Log::LogInfo(const wchar_t* message)
	{
		m_Logs.emplace(std::wstring(L"[INFO]: ") + message);
	}

	void Log::LogWarning(const wchar_t* message)
	{
		m_Logs.emplace(std::wstring(L"[WARNING]: ") + message);
	}

	void Log::LogError(const wchar_t* message)
	{
		m_Logs.emplace(std::wstring(L"[ERROR]: ") + message);
	}

	std::wstring& Log::GetLastLogLine()
	{
		return m_Logs.front();
	}

	void Log::PopLastLogLine()
	{
		m_Logs.pop();
	}
}