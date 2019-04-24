#pragma once

#include "LayerStack.h"
#include "Window.h"

namespace Soul {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// TO BE DEFINED BY CLIENT
	Application* CreateApplication();
}