#pragma once

#include "Soul/Soul.h"

#include <functional>

#include "GameLayer.h"

class MainMenuLayer : public Soul::Layer
{
public:
	MainMenuLayer()
		: Soul::Layer(160, 45),
		m_UI()
	{
		Soul::UIButton* startButton = new Soul::UIButton("Start Game", 74, 15, [this]()
		{
			Soul::LayerStack::QueueAction(Soul::LayerStack::QueuePopLayer, this);
			Soul::LayerStack::QueueAction(Soul::LayerStack::QueuePushLayer,
				new GameLayer(new Soul::ChartFile("Songs/Soria Moria/notes")));
		});

		Soul::UIButton* quitButton = new Soul::UIButton("Quit Game", 74, 17, [this]()
		{
			Soul::LayerStack::QueueAction(Soul::LayerStack::QueueFullClear, this);
		});

		startButton->AddConnection(quitButton, Soul::Down);
		startButton->AddConnection(quitButton, Soul::Up);
		quitButton->AddConnection(startButton, Soul::Down);
		quitButton->AddConnection(startButton, Soul::Up);

		m_UI.AddUIElement(startButton);
		m_UI.AddUIElement(quitButton);

		for (int i = 0; i < m_BufferSize; i++)
			m_Draw[i] = ' ';
	}

	bool Update(double deltaTime) override
	{
		m_UI.Update(deltaTime);
		return true;
	}

	const char* Draw() const override
	{
		const std::vector<Soul::UIElement*>& elements = m_UI.Draw();

		for (const Soul::UIElement* element : elements)
			DrawToBuffer(element->Draw(), element->GetXPos(), element->GetYPos());

		return m_Draw;
	}

private:
	Soul::UISet m_UI;
};