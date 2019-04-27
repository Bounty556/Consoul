#pragma once

#include "InputManager.h"

namespace Soul {
	class UIElement
	{
	public:
		explicit UIElement(int xPos, int yPos)
			: m_XPos(xPos), m_YPos(yPos) {}

		void AddConnection(UIElement* connection, Keys key)
		{
			m_Connected.insert(std::pair<Keys, UIElement*>(key, connection));
		}

		bool CheckUITraversal()
		{
			if (IsSelected)
			{
				for (auto it = m_Connected.begin(); it != m_Connected.end(); ++it)
				{
					if (InputManager::WasKeyPressed((*it).first))
					{
						Unselect();
						(*it).second->Select();
						return false;
					}
				}
			}

			return true;
		}

		// Returns true if other UI Elements should update
		virtual void Update(float deltaTime) { };
		
		virtual void Select() = 0;

		inline const wchar_t* Draw() const { return m_Draw; };
		inline const int GetXPos() const { return m_XPos; }
		inline const int GetYPos() const { return m_YPos; }
	protected:
		virtual void Unselect() = 0;
	protected:
		int m_XPos;
		int m_YPos;
		wchar_t* m_Draw;
		bool IsSelected = false;
		std::map<Keys, UIElement*> m_Connected;
	};
}