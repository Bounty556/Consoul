#include "pch.h"
#include "UISet.h"

namespace Soul {
	UISet::UISet()
	{
		InputManager::AddKey(Up);
		InputManager::AddKey(Down);
		InputManager::AddKey(Left);
		InputManager::AddKey(Right);
	}

	UISet::~UISet()
	{
		for (auto it = m_Elements.begin(); it != m_Elements.end(); ++it)
			delete (*it);

		m_Elements.clear();
	}

	void UISet::AddUIElement(UIElement* element)
	{
		if (m_Elements.empty())
			element->Select();
		m_Elements.emplace_back(element);
	}

	void UISet::RemoveUIElement(UIElement* element)
	{
		auto found = std::find(m_Elements.begin(), m_Elements.end(), element);

		if (found != m_Elements.end())
			m_Elements.erase(found);
	}

	void UISet::Update(double deltaTime)
	{
		bool checkInput = true;
		for (auto it = m_Elements.begin(); it != m_Elements.end(); ++it)
		{
			if (checkInput)
				checkInput = (*it)->CheckUITraversal();

			(*it)->Update(deltaTime);
		}
	}

	const std::vector<UIElement*>& UISet::Draw() const
	{
		return m_Elements;
	}
}