#include "pch.h"
#include "InputManager.h"

namespace Soul {
	std::map<Keys, unsigned char> InputManager::m_KeyState;

	void InputManager::AddKey(Keys key)
	{
		m_KeyState.insert(std::pair<Keys, unsigned char>(key, State::None));
	}

	bool InputManager::IsKeyDown(Keys key)
	{
		auto found = m_KeyState.find(key);

		if (found == m_KeyState.end())
			return false;

		return found->second & State::Down;
	}

	bool InputManager::WasKeyPressed(Keys key)
	{
		auto found = m_KeyState.find(key);

		if (found == m_KeyState.end())
			return false;

		return found->second & State::Pressed;
	}

	bool InputManager::WasKeyReleased(Keys key)
	{
		auto found = m_KeyState.find(key);

		if (found == m_KeyState.end())
			return false;

		return found->second & State::Released;
	}

	void InputManager::UpdateStates()
	{
		for (auto it = m_KeyState.begin(); it != m_KeyState.end(); ++it)
		{
			if (GetAsyncKeyState(it->first))
			{
				if (it->second == State::None)
					it->second = State::Down | State::Pressed;
				else
					it->second = State::Down;
			}
			else
			{
				// Set as State::Released for the first frame, then none for the rest
				if (it->second & State::Down)
					it->second = State::Released;
				else
					it->second = State::None;
			}
				
		}
	}
}