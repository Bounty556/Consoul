#include "pch.h"
#include "Soul/Soul.h"

///TODO: MAKE GAME LAYER

class ExampleUILayer : public Soul::Layer
{
public:
	ExampleUILayer()
		: Layer(160, 45),
		m_UI(new Soul::UISet())
	{
		Soul::UIButton* testTop = new Soul::UIButton("This is test Top", 30, 10,
			[]() { Soul::Log::LogInfo("Top Button Pressed!"); });

		Soul::UIButton* testMiddle = new Soul::UIButton("This is test Middle", 30, 12,
			[]() { Soul::Log::LogInfo("Middle Button Pressed!"); });

		Soul::UIButton* testBottom = new Soul::UIButton("This is test Bottom", 30, 14,
			[]() { Soul::Log::LogInfo("Bottom Button Pressed!"); });

		testTop->AddConnection(testBottom, Soul::Up);
		testTop->AddConnection(testMiddle, Soul::Down);

		testMiddle->AddConnection(testTop, Soul::Up);
		testMiddle->AddConnection(testBottom, Soul::Down);

		testBottom->AddConnection(testMiddle, Soul::Up);
		testBottom->AddConnection(testTop, Soul::Down);

		m_UI->AddUIElement(testTop);
		m_UI->AddUIElement(testMiddle);
		m_UI->AddUIElement(testBottom);

		for (int i = 0; i < m_BufferSize; i++)
			m_Draw[i] = ' ';
	}

	bool Update(float deltaTime) override
	{
		m_UI->Update(deltaTime);
		return true;
	}

	const char* Draw() const override
	{
		const std::vector<Soul::UIElement*>& set = m_UI->Draw();

		for (auto it = set.begin(); it != set.end(); ++it)
			DrawToBuffer((*it)->Draw(), (*it)->GetXPos(), (*it)->GetYPos());

		return m_Draw;
	}

private:
	Soul::UISet* m_UI;
};

class Client : public Soul::Application
{
public:
	Client()
	{
		PushLayer(new ExampleUILayer());
		PushOverlay(new Soul::ConsoleLayer(160, 6));

		Soul::ChartFile file("hola");
	}

	~Client()
	{
	}
};

Soul::Application* Soul::CreateApplication()
{
	return new Client();
}