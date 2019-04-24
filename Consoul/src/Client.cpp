#include "pch.h"
#include "Soul/Soul.h"

/// TODO: ADD LAYERS
/// TODO: ADD CONSOLE LOG
/// TODO: ADD INPUT MANAGER

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 45;

class Client : public Soul::Application
{
public:
	Client()
	{

	}

	~Client()
	{

	}
};

Soul::Application* Soul::CreateApplication()
{
	return new Client();
}