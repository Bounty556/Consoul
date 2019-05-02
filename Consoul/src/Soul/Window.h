#pragma once

namespace Soul {
	class Window
	{
	public:
		Window(int width, int height);
		~Window();

		void DrawToBuffer(const char* glyph, int length, int x, int y) const;

		void ClearFrame() const;
		void DrawFrame();

		inline int GetWidth() const { return m_BufferWidth; }
		inline int GetHeight() const { return m_BufferHeight; }
		inline int GetSize() const { return m_BufferSize; }

		static Window* Create();
	private:
		int m_BufferWidth;
		int m_BufferHeight;
		int m_BufferSize;
		mutable char* m_Screen;
		HANDLE m_Console;
		DWORD m_Bytes;
	};
}