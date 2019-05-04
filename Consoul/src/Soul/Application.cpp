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

		while (m_Running)
		{
			currentFrame = std::chrono::high_resolution_clock::now();
			elapsedTime = currentFrame - previousFrame;
			previousFrame = currentFrame;

			deltaTime = elapsedTime.count();

			m_LayerStack.Update(deltaTime);
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