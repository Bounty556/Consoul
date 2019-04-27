#include "pch.h"
#include "Soul/Soul.h"

///TODO: MAKE FRAMETIMES ACTUALLY WORK
///TODO: FIX RENDERING WRAPPING AROUND SCREEN
///TODO: MAKE LAYERSTACK WORK IN QUEUES
///TODO: MAKE GAME LAYER

class ExampleUILayer : public Soul::Layer
{
public:
	ExampleUILayer()
		: Layer(160, 40),
		m_UI(new Soul::UISet())
	{
		Soul::UIButton* testTop = new Soul::UIButton(L"This is test Top", 30, 10,
			[]() { Soul::Log::LogInfo(L"Top Button Pressed!"); });

		Soul::UIButton* testMiddle = new Soul::UIButton(L"This is test Middle", 30, 12,
			[]() { Soul::Log::LogInfo(L"Middle Button Pressed!"); });

		Soul::UIButton* testBottom = new Soul::UIButton(L"This is test Bottom", 30, 14,
			[]() { Soul::Log::LogInfo(L"Bottom Button Pressed!"); });

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

	void Update(float deltaTime) override
	{
		m_UI->Update(deltaTime);
	}

	const wchar_t* Draw() const override
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
		PushOverlay(new Soul::ConsoleLayer(160, 6));
		PushLayer(new ExampleUILayer());
	}

	~Client()
	{
	}
};

Soul::Application* Soul::CreateApplication()
{
	return new Client();
}