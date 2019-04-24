#pragma once

extern Soul::Application* Soul::CreateApplication();

int main()
{
	auto app = Soul::CreateApplication();
	app->Run();
	delete app;
}