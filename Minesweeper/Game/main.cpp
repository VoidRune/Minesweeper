#include "Application.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	Application* app = new Application("Minesweeper", 640, 640);
	app->Run();
	delete app;
	return 0;
}