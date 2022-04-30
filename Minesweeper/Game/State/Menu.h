#pragma once
#include "Graphics/UI/Widgets.h"
#include "Game.h"
#include "Graphics/Camera/Camera2D.h"
#include "Graphics/UI/Font.h"
#include <vector>

class MainMenu
{
public:
	MainMenu(Game* game, int* gameState);
	~MainMenu();

	void Update();
	 
private:
	void ReadSettings();
	void WriteSettings();

	enum class State
	{
		MainMenu = 0,
		PlayMenu,
		CustomGame,
		Campaign,
		Settings
	};

	int customWidth = 16;
	int customHeight = 16;
	int customProbability = 20;

	std::string seed;
	bool isStringBoxEnabled = false;
	uint32_t campaignLevel = 1;

	uint32_t windowWidth = 6; /* Is multiplied with 128 */
	bool showMinimap = true;
	uint32_t audioVolume = 100;

	TextureBatch texBatch;
	Sound click;
	vec4f bg, bg_play, bg_custom, bg_campaign, bg_settings;
	vec4f button, button_hover;
	vec4f title;

	Camera2D cam;

	State menuState;
	int* gameState;
	//to start the game
	Game* game;
};