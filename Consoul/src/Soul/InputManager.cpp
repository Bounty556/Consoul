#include "pch.h"
#include "InputManager.h"

namespace Soul {
	std::map<Keys, InputManager::State> InputManager::m_KeyStates;
	bool InputManager::m_InputDetected;
	std::vector<Keys> InputManager::m_NoteKeys;

	void InputManager::AddKey(Keys key)
	{
		m_KeyStates.insert(std::pair<Keys, InputManager::State>(key, State::None));
	}

	void InputManager::AddNoteKey(Keys key)
	{
		m_NoteKeys.push_back(key);
	}

	bool InputManager::IsKeyDown(Keys key)
	{
		auto found = m_KeyStates.find(key);

		if (found == m_KeyStates.end())
			return false;

		return found->second & State::Down;
	}

	bool InputManager::WasKeyPressed(Keys key)
	{
		auto found = m_KeyStates.find(key);

		if (found == m_KeyStates.end())
			return false;

		return found->second & State::Pressed;
	}

	bool InputManager::WasKeyReleased(Keys key)
	{
		auto found = m_KeyStates.find(key);

		if (found == m_KeyStates.end())
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
		// Loop through all keys registered and mark whether
		for (auto it = m_KeyStates.begin(); it != m_KeyStates.end(); ++it)
		{
			// The state the note was in before we changed it
			State lastNoteState = (State)(it->second);

			// Is the key down
			if (GetAsyncKeyState(it->first))
			{
				// If the key was not pressed
				if (it->second == State::None)
				{
					// Mark it as down and just pressed
					it->second = (InputManager::State)(State::Down | State::Pressed);
				}
				else
				{
					// Otherwise just mark it as down
					it->second = State::Down;
				}
			}
			else
			{
				// Set as State::Released for the first frame, then none for the rest
				if (it->second & State::Down)
				{
					it->second = State::Released;
				}
				else
				{
					it->second = State::None;
				}
			}
			
			// If we have a note that was pressed, mark input as detected
			if (lastNoteState != it->second
				&& std::find(m_NoteKeys.begin(), m_NoteKeys.end(), it->first) != m_NoteKeys.end())
			{
				m_InputDetected = true;
			}
		}
	}

	short InputManager::GetKeysDown()
	{
		short keysDown = 0;

		for (auto it = m_KeyStates.begin(); it != m_KeyStates.end(); ++it)
			if (it->second & State::Down)
				keysDown += it->first;
		
		return keysDown;
	}
}