#pragma once

#include "InputManager.h"
#include "UIElement.h"

namespace Soul {
	class UISet
	{
	public:
		UISet();
		~UISet();

		void AddUIElement(UIElement* element);
		void RemoveUIElement(UIElement* element);

		void Update(float deltaTime);

		const std::vector<UIElement*>& Draw() const;
	private:
		std::vector<UIElement*> m_Elements;
	};
}