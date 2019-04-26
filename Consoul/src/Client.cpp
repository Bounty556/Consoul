#include "pch.h"
#include "Soul/Soul.h"

///TODO: ADD BUTTONLIST
///TODO: ADD BUTTONS
///TODO: FIX RENDERING WRAPPING AROUND SCREEN

class Client : public Soul::Application
{
public:
	Client()
	{
		PushLayer(new Soul::ConsoleLayer(160, 6));

		Soul::Log::LogInfo(L"Hello!");
	}

	~Client()
	{
	}
};

Soul::Application* Soul::CreateApplication()
{
	return new Client();
}