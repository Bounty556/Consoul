#pragma once

#include "UIElement.h"

namespace Soul {
	class UIButton : public UIElement
	{
	public:
		explicit UIButton(const char* text, int xPos, int yPos, std::function<void()> callback);

		virtual void Update(double deltaTime) override;
	protected:
		virtual void Select() override;
		virtual void Unselect() override;
	private:
		std::function<void()> m_Callback;
	};
}