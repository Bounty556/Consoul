#pragma once

namespace Soul {
	class Log
	{
		Log() = delete;
	public:
		static void LogInfo(const char* message);
		static void LogWarning(const char* message);
		static void LogError(const char* message);
		static void LogInfo(const std::string& message);
		static void LogWarning(const std::string& message);
		static void LogError(const std::string& message);
		static const std::string& GetLastLogLine();
		static void PopLastLogLine();
		static inline bool IsAccessible() { return !m_Logs.empty(); }
	private:
		static std::queue<std::string> m_Logs;
	};
}