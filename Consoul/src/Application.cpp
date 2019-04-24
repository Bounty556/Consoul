#include "pch.h"
#include "Soul/Window.h"

/// TODO: ADD LAYERS
/// TODO: ADD CONSOLE LOG
/// TODO: ADD INPUT MANAGER
/// TODO: ADD EVENTS

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 45;

int main()
{
	Soul::Window window(SCREEN_WIDTH, SCREEN_HEIGHT);

	while (window.IsRunning())
	{
		for (int x = 0; x < window.GetWidth(); ++x)
			for (int y = 0; y < window.GetHeight(); ++y)
				window.DrawToBuffer('#', x, y);

		window.DrawToBuffer(L"Hello There!!", 13, 50, 10);

		window.DrawFrame();
	}
	return 0;
}