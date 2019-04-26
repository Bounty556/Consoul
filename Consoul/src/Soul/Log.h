#pragma once

namespace Soul {
	class Log
	{
		Log() = delete;
		~Log() = delete;
	public:
		static void LogInfo(const wchar_t* message);
		static void LogWarning(const wchar_t* message);
		static void LogError(const wchar_t* message);
		static std::wstring& GetLastLogLine();
		static void PopLastLogLine();
		static inline bool IsAccessible() { return !m_Logs.empty(); }
	private:
		static std::queue<std::wstring> m_Logs;
	};
}