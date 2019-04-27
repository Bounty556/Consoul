#pragma once

#include "UIElement.h"

namespace Soul {
	class UIButton : public UIElement
	{
	public:
		explicit UIButton(const wchar_t* text, int xPos, int yPos, std::function<void()> callback);

		virtual void Update(float deltaTime) override;
	protected:
		virtual void Select() override;
		virtual void Unselect() override;
	private:
		std::function<void()> m_Callback;
	};
}