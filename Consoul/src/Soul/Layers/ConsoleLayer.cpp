#include "pch.h"
#include "ConsoleLayer.h"

#include "InputManager.h"
#include "Log.h"

namespace Soul {
	ConsoleLayer::ConsoleLayer(int width, int height)
		: Layer(width, height)
	{
		Soul::InputManager::AddKey(Soul::Tilde);

		// Draw the bounding box for the console
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

	void ConsoleLayer::Update(float deltaTime)
	{
		if (Soul::InputManager::WasKeyPressed(Soul::Tilde))
			IsDrawn = !IsDrawn;

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
}