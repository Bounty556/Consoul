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
			m_LayerStack.Update(0.001f);
			m_Window->ClearFrame();
			m_LayerStack.Draw(m_Window.get());
			m_Window->DrawFrame();
		}
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}
}