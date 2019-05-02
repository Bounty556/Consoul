#include "pch.h"
#include "Log.h"

namespace Soul {
	std::queue<std::string> Log::m_Logs;

	void Log::LogInfo(const char* message)
	{
		m_Logs.emplace(std::string("[INFO]: ") + message);
	}

	void Log::LogWarning(const char* message)
	{
		m_Logs.emplace(std::string("[WARNING]: ") + message);
	}

	void Log::LogError(const char* message)
	{
		m_Logs.emplace(std::string("[ERROR]: ") + message);
	}
	
	void Log::LogInfo(const std::string& message)
	{
		m_Logs.emplace(std::string("[INFO]: ") + message);
	}

	void Log::LogWarning(const std::string& message)
	{
		m_Logs.emplace(std::string("[WARNING]: ") + message);
	}

	void Log::LogError(const std::string& message)
	{
		m_Logs.emplace(std::string("[ERROR]: ") + message);
	}

	const std::string& Log::GetLastLogLine()
	{
		return m_Logs.front();
	}

	void Log::PopLastLogLine()
	{
		m_Logs.pop();
	}
}