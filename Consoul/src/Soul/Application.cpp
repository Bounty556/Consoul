#include "pch.h"
#include "Application.h"
#include "InputManager.h"
#include "Audio/AudioEngine.h"

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
		std::chrono::steady_clock::time_point previousFrame = std::chrono::high_resolution_clock::now();
		std::chrono::steady_clock::time_point currentFrame;
		std::chrono::duration<double> elapsedTime;
		double deltaTime;
		int modifier = 1;

		InputManager::AddKey(One);
		InputManager::AddKey(Two);

		while (m_Running)
		{
			currentFrame = std::chrono::high_resolution_clock::now();
			elapsedTime = currentFrame - previousFrame;
			previousFrame = currentFrame;

			deltaTime = elapsedTime.count();

			if (InputManager::WasKeyPressed(One))
			{
				if (--modifier < 1)
					modifier = 1;

				AudioEngine::SetSpeed(modifier);
			}
			else if (InputManager::WasKeyPressed(Two))
			{
				AudioEngine::SetSpeed(++modifier);
			}

			m_LayerStack.Update(deltaTime / modifier);
			InputManager::UpdateStates();
			m_Window->ClearFrame();
			m_LayerStack.Draw(m_Window.get());
			m_Window->DrawFrame();
		}

		AudioEngine::CleanUp();
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.QueueAction(LayerStack::Action::QueuePushLayer, layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.QueueAction(LayerStack::Action::QueuePushOverlay, overlay);
	}
}