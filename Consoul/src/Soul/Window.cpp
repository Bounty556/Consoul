#include "pch.h"
#include "Window.h"

namespace Soul {
	Window::Window(int width, int height)
		: m_BufferWidth(width), m_BufferHeight(height), m_BufferSize(width * height),
		m_Screen(new wchar_t[m_BufferSize]),
		m_Bytes(0),
		m_Running(true)
	{
		m_Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,
			CONSOLE_TEXTMODE_BUFFER, NULL);

		// Error handling
		if (m_Console == INVALID_HANDLE_VALUE)
		{
			m_Running = false;
			swprintf_s(m_Screen, 160, L"ERROR [%d]: Could not create window", GetLastError());
		}
		else
		{
			SetConsoleActiveScreenBuffer(m_Console);
		}
	}

	Window::~Window()
	{
		CloseHandle(m_Console);
		delete[] m_Screen;
	}

	void Window::DrawToBuffer(wchar_t wchar, int x, int y)
	{
		if (x < 0 || x >= m_BufferWidth || y < 0 || y >= m_BufferHeight)
		{
			// Report error
			return;
		}

		m_Screen[y * m_BufferWidth + x] = wchar;
	}

	void Window::DrawToBuffer(const wchar_t* wstring, int length, int x, int y)
	{
		if (x < 0 || x >= m_BufferWidth || y < 0 || y >= m_BufferHeight ||
			length + y * m_BufferWidth + x >= m_BufferSize)
		{
			// Report error
			return;
		}

		for (int i = 0; i < length; ++i)
			m_Screen[i + y * m_BufferWidth + x] = wstring[i];
	}

	void Window::DrawFrame()
	{
		m_Screen[m_BufferWidth * m_BufferHeight - 1] = '\0';
		WriteConsoleOutputCharacterW(m_Console, m_Screen, m_BufferSize, { 0, 0 }, &m_Bytes);
	}

	void Window::Update(float deltaTime)
	{
	}
}