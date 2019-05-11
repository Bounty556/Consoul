#include "pch.h"
#include "InputManager.h"

namespace Soul {
	std::map<Keys, unsigned char> InputManager::m_KeyState;
	bool InputManager::m_InputDetected;
	std::vector<Keys> InputManager::m_TrackedKeys;

	void InputManager::AddKey(Keys key)
	{
		m_KeyState.insert(std::pair<Keys, unsigned char>(key, State::None));
	}

	void InputManager::AddTrackedKey(Keys key)
	{
		m_TrackedKeys.push_back(key);
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

	bool InputManager::IsInputChangeDetected()
	{
		return m_InputDetected;
	}

	void InputManager::UpdateStates()
	{
		m_InputDetected = false;
		for (auto it = m_KeyState.begin(); it != m_KeyState.end(); ++it)
		{
			State notePrev = (State)(it->second);

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
			
			if (!m_InputDetected && notePrev != it->second
				&& std::find(m_TrackedKeys.begin(), m_TrackedKeys.end(), it->first) != m_TrackedKeys.end())
				m_InputDetected = true;
		}
	}

	short InputManager::GetKeysDown()
	{
		short keysDown = 0;

		for (auto it = m_KeyState.begin(); it != m_KeyState.end(); ++it)
			if (it->second & State::Down)
				keysDown += it->first;
		
		return keysDown;
	}
}