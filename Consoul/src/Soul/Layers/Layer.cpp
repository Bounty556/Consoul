#include "pch.h"
#include "Layer.h"

namespace Soul {
	Layer::Layer(int width, int height, int xPos /*= 0*/, int yPos /*= 0*/)
		: m_BufferWidth(width), m_BufferHeight(height), m_BufferSize(width * height),
		m_XPos(xPos), m_YPos(yPos),
		m_Draw(new char[m_BufferSize])
	{
	}

	Layer::~Layer()
	{
		delete[] m_Draw;
	}

	const char* Layer::Draw() const
	{
		return m_Draw;
	}

	void Layer::DrawToBuffer(const char* string, int x, int y) const
	{
		int length = strlen(string);
		for (int i = 0; i < length; i++)
			m_Draw[y * m_BufferWidth + x + i] = string[i];
	}

	void Layer::ClearBuffer()
	{
		for (int i = 0; i < m_BufferSize; i++)
			m_Draw[i] = ' ';
	}
}