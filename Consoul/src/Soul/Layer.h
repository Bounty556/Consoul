#pragma once

namespace Soul {
	class Layer
	{
	public:
		Layer() = delete;
		Layer(int width, int height, int xPos = 0, int yPos = 0);
		virtual ~Layer();

		virtual void Update(float deltaTime) { };
		virtual const wchar_t* Draw() const;

		inline int GetXPos() const { return m_XPos; }
		inline int GetYPos() const { return m_YPos; }
		inline int GetLength() const { return m_BufferSize; }
	protected:
		void DrawToBuffer(const wchar_t* wstring, int length, int x, int y);
		void ClearBuffer();
	public:
		bool IsDrawn = true;
	protected:
		int m_BufferWidth;
		int m_BufferHeight;
		int m_BufferSize;
		int m_XPos;
		int m_YPos;
		wchar_t* m_Draw;
	};
}