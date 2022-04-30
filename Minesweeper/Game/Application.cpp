#include "Application.h"
#include <iostream>

Application::Application(const char* name, float width, float height) :
	win( new Window(name, false, { width, height })),
	device( new GraphicsDevice(win.get(), true)),
	renderer(new Renderer),
	audioEngine(new AudioEngine),
	game(new Game(&gameState)),
	mainMenu(new MainMenu(game.get(), &gameState)),
	camera(new Camera2D),
	resourceManager(new ResourceManager)
{
	gameState = 0;
}

void Application::Run()
{
	while (win->ProcessInput())
	{
		device->ClearBuffer();
		device->ClearDepth();

		switch (gameState)
		{
			/* game */
		case 1:
			game->Update();
		break;

			/* Main menu */
		case 0:
			mainMenu->Update();
		break;
		}
		Renderer::Get().Draw();
		device->SwapBuffer();
	}
}

Application::~Application()
{

}