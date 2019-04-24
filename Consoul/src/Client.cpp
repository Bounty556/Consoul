#include "pch.h"
#include "Soul/Soul.h"

/// TODO: ADD CONSOLE LOG
/// TODO: ADD INPUT MANAGER

class ExampleLayer : public Soul::Layer
{
public:
	ExampleLayer(int width, int height)
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
};

class Client : public Soul::Application
{
public:
	Client()
	{
		PushLayer(new ExampleLayer(160, 6));
	}

	~Client()
	{

	}
};

Soul::Application* Soul::CreateApplication()
{
	return new Client();
}