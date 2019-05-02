#include "pch.h"
#include "UIButton.h"

namespace Soul {
	UIButton::UIButton(const char* text, int xPos, int yPos, std::function<void()> callback)
		: UIElement(xPos, yPos),
		m_Callback(callback)
	{
		InputManager::AddKey(Enter);

		char length = (char)strlen(text);
		m_Draw = new char[length + 2];
		m_Draw[0] = ' ';
		m_Draw[length + 1] = '\0';
		for (int i = 0; i < length; ++i)
			m_Draw[i + 1] = text[i];
	}

	void UIButton::Update(float deltaTime)
	{
		if (IsSelected && InputManager::WasKeyPressed(Enter))
			m_Callback();
	}

	void UIButton::Select()
	{
		IsSelected = true;

		m_Draw[0] = '>';
	}

	void UIButton::Unselect()
	{
		IsSelected = false;

		m_Draw[0] = ' ';
	}
}