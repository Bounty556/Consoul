#include "pch.h"
#include "Application.h"

namespace Soul {
	Application::Application()
		: m_Window(Window::Create())
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->Update(0.001f);
			m_Window->ClearFrame();
			m_Window->DrawToBuffer(L"HELLLOOOOOO", 11, 20, 40);
			m_Window->DrawFrame();
		}
	}
}