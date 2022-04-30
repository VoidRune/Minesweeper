#pragma once
#include <memory>
#include "State/Menu.h"
#include "State/Game.h"
#include "Graphics/ResourceManager.h"
#include "Utility/Helper.h"

class Application
{
public:
	Application(const char* name, float width, float height);
	~Application();

	void Run();
private:

	/* Core */
	std::unique_ptr<Window> win;
	std::unique_ptr<GraphicsDevice> device;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<AudioEngine> audioEngine;
	std::unique_ptr<Camera2D> camera;
	std::unique_ptr<ResourceManager> resourceManager;

	/* Game */
	int gameState;
	std::unique_ptr<Game> game;
	std::unique_ptr<MainMenu> mainMenu;
};