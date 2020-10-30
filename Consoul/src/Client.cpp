#include "pch.h"
#include "Soul/Soul.h"
#include "MainMenuLayer.h"

// TODO: ADD SONG PICKER MENU
// TODO: ADD GAME OVER STATE (SONG IS DONE)
// TODO: SAVE SCORES
// TODO: RATE SCORES
// TODO: ADD SUSTAIN NOTES
// TODO: VARIABLE VIDEO OFFSET
// TODO: VARIABLE AUDIO OFFSET

class Client : public Soul::Application
{
public:
	Client()
	{
		Soul::LayerStack::QueueAction(Soul::LayerStack::QueuePushOverlay, new Soul::ConsoleLayer(160, 6));
		Soul::LayerStack::QueueAction(Soul::LayerStack::QueuePushLayer, new MainMenuLayer());
	}

	~Client()
	{
	}
};

Soul::Application* Soul::CreateApplication()
{
	return new Client();
}