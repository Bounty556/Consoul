#pragma once

namespace Soul {
	class Window
	{
	public:
		Window(int width, int height);
		~Window();

		void DrawToBuffer(wchar_t glyph, int x, int y);
		void DrawToBuffer(const wchar_t* glyph, int length, int x, int y);

		void DrawFrame();
		void Update(float deltaTime);

		inline int GetWidth() const { return m_BufferWidth; }
		inline int GetHeight() const { return m_BufferHeight; }
		inline int GetSize() const { return m_BufferSize; }
		inline int IsRunning() const { return m_Running; }
	private:
		int m_BufferWidth;
		int m_BufferHeight;
		int m_BufferSize;
		wchar_t* m_Screen;
		HANDLE m_Console;
		DWORD m_Bytes;
		bool m_Running;
	};
}