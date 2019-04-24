#pragma once

#include "Window.h"

namespace Soul {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// TO BE DEFINED BY CLIENT
	Application* CreateApplication();
}