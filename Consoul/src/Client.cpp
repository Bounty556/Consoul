#include "pch.h"
#include "Soul/Soul.h"

/// TODO: ADD INPUT MANAGER

class ConsoleLayer : public Soul::Layer
{
public:
	ConsoleLayer(int width, int height)
		: Layer(width, height, 0, 0)
	{
		for (int x = 0; x < m_BufferWidth; x++)
		{
			for (int y = 0; y < m_BufferHeight; y++)
			{
				if (x == 0 || x == m_BufferWidth - 1)
					m_Draw[y * m_BufferWidth + x] = '|';
				else if (y == 0 || y == m_BufferHeight - 1)
					m_Draw[y * m_BufferWidth + x] = '-';
				else
					m_Draw[y * m_BufferWidth + x] = ' ';
			}
		}
	}

	void Update(float deltaTime) override
	{
		while (Soul::Log::IsAccessible())
		{
			// Move All lines back
			for (int i = 1; i < m_BufferHeight - 2; ++i)
				for (int j = 1; j < m_BufferWidth - 1; ++j)
					m_Draw[i * m_BufferWidth + j] = m_Draw[(i + 1) * m_BufferWidth + j];

			// Write last written log line into console and write the rest of the space as blank
			std::wstring& logLine = Soul::Log::GetLastLogLine();
			for (int i = 1; i < m_BufferWidth - 1; ++i)
			{
				if (i - 1 < logLine.length())
					m_Draw[(m_BufferHeight - 2) * m_BufferWidth + i] = logLine[i - 1];
				else
					m_Draw[(m_BufferHeight - 2) * m_BufferWidth + i] = ' ';
			}
			Soul::Log::PopLastLogLine();
		}
	}
};

class Client : public Soul::Application
{
public:
	Client()
	{
		PushLayer(new ConsoleLayer(160, 6));
		Soul::Log::LogInfo(L"There are 366 days in a leap year!");
		Soul::Log::LogWarning(L"There are enemies nearby!");
		Soul::Log::LogError(L"Your legs are gone!");
	}

	~Client()
	{

	}
};

Soul::Application* Soul::CreateApplication()
{
	return new Client();
}