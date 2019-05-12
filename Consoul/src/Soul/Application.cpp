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

			LayerStack::Update(deltaTime / modifier);
			InputManager::UpdateStates();
			m_Window->ClearFrame();
			LayerStack::Draw(m_Window.get());
			m_Window->DrawFrame();

			if (LayerStack::AllEmpty())
				m_Running = false;
		}

		LayerStack::CleanUp();
		AudioEngine::CleanUp();
	}
}