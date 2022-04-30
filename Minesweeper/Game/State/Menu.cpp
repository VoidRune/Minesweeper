#include "Menu.h"
#include "Graphics/Renderer.h"
#include "Window/Window.h"
#include "Window/Input.h"
#include "Core/Network/Networking.h"

MainMenu::MainMenu(Game* game, int* gameState) :
	game(game), gameState(gameState)
{
	texBatch.AddTexture("res/Textures/background.png");
	texBatch.AddTexture("res/Textures/background_play.png");
	texBatch.AddTexture("res/Textures/background_custom.png");
	texBatch.AddTexture("res/Textures/background_campaign.png");
	texBatch.AddTexture("res/Textures/background_settings.png");
	texBatch.AddTexture("res/Textures/Menu.png");
	texBatch.AddTexture("res/Textures/button.png");
	texBatch.AddTexture("res/Textures/button_hover.png");
	texBatch.AddTexture("res/Textures/title.png");
	texBatch.Finish();

	ResourceManager::Get().AddTextureBatch("uiTex", texBatch);

	click = AudioEngine::Get().LoadData("res/Audio/button_click.ogg");
	bg = texBatch.get_coords("res/Textures/background.png");
	bg_play = texBatch.get_coords("res/Textures/background_play.png");
	bg_custom = texBatch.get_coords("res/Textures/background_custom.png");
	bg_campaign = texBatch.get_coords("res/Textures/background_campaign.png");
	bg_settings = texBatch.get_coords("res/Textures/background_settings.png");
	button = texBatch.get_coords("res/Textures/button.png");
	button_hover = texBatch.get_coords("res/Textures/button_hover.png");
	title = texBatch.get_coords("res/Textures/title.png");
	ResourceManager::Get().AddSound("click", click);
	menuState = State::MainMenu;
	
	ReadSettings();
	//GraphicsDevice::Get().SetFullscreen(isFullscreen);
	Window::Get().ResizeWindow(windowWidth * 128, windowWidth * 128);
	AudioEngine::Get().SetMasterVolume(audioVolume / 100.0f);

	game->LoadBestScores(&campaignLevel, &showMinimap);
	seed.reserve(10);
}

MainMenu::~MainMenu()
{

}

void MainMenu::Update()
{

	switch (menuState)
	{
	/* Main menu */
	case State::MainMenu:
	{
		gui::Sprite(0.5f, 0.5f, 5.0f, 1.0f, 1.0f, bg);
		gui::Sprite(0.5f, 0.85f, 4.0f, 0.8f, 0.2f, title);

		if (gui::Button("Play", 0.5f, 0.6f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			menuState = State::PlayMenu;
		}
		else if (gui::Button("Campaign", 0.5f, 0.4f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			menuState = State::Campaign;
		}
		else if (gui::Button("Options", 0.5f, 0.2f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			menuState = State::Settings;
		}
		else if (gui::Button("X", 0.8f, 0.2f, 0.085f, 0.085f, {0.1f, 0.1f, 0.1f}))
		{
			Window::Get().CloseWindow();
		}

		break;
	}
	/* Play menu */
	case State::PlayMenu:
	{

		Renderer::Get().AddTexturedQuad(0.5f, 0.5f, 5.0f, 1.0f, 1.0f, bg_play);

		if (gui::Button("Easy", 0.3f, 0.8f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			*gameState = 1;
			game->Start(8, 8, 0.18f, 0, 0);
		}
		else if (gui::Button("Normal", 0.7f, 0.8f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			*gameState = 1;
			game->Start(12, 12, 0.20f, 0, 1);
		}
		else if (gui::Button("Medium", 0.3f, 0.6f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			*gameState = 1;
			game->Start(16, 16, 0.22f, 0, 2);
		}
		else if (gui::Button("Hard", 0.7f, 0.6f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			*gameState = 1;
			game->Start(24, 24, 0.25f, 0, 3);
		}
		else if (gui::Button("Custom", 0.5f, 0.4f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			menuState = State::CustomGame;
		}
		else if (gui::Button("Back", 0.5f, 0.2f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			menuState = State::MainMenu;
		}
		break;
	}
	/* Custom game */
	case State::CustomGame:
	{
		Renderer::Get().AddTexturedQuad(0.5f, 0.5f, 5.0f, 1.0f, 1.0f, bg_custom);

		gui::Slider(&customWidth, 4, 60, 0.5f, 0.9f, 0.8f, 0.02f, { 0.2f, 0.2f, 0.2f });
		gui::Text("width: " + numToString(customWidth), 0.5f, 0.85f, -6.0f);

		gui::Slider(&customHeight, 4, 60, 0.5f, 0.8f, 0.8f, 0.02f, { 0.2f, 0.2f, 0.2f });
		gui::Text("Height: " + numToString(customHeight), 0.5f, 0.75f, -6.0f);

		gui::Slider(&customProbability, 1, 100, 0.5f, 0.7f, 0.8f, 0.02f, { 0.2f, 0.2f, 0.2f });
		gui::Text("Probability: " + numToString(customProbability) + "%", 0.5f, 0.65f, -6.0f);

		gui::InputStringBox(seed, &isStringBoxEnabled, 10, 0.55, 0.55, 0.45, 0.1, { 0.2f, 0.2f, 0.2f });
		gui::Text("Seed:", 0.22f, 0.55f, -6.0f);
		
		if (gui::Button("Play", 0.5f, 0.4f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			*gameState = 1;
			game->Start(customWidth, customHeight, customProbability / 100.0f, (seed == "") ? 0 : (int32_t)(std::hash<std::string>{}(seed)), 4);
		}
		else if (gui::Button("Back", 0.5f, 0.2f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			menuState = State::PlayMenu;
		}
		break;
	}
	/* Campaign */
	case State::Campaign:
	{
		Renderer::Get().AddTexturedQuad(0.5f, 0.5f, 5.0f, 1.0f, 1.0f, bg_campaign);
		
		int level;
		if (gui::ButtonArray(&level, 1, campaignLevel, 6, 5, 0.5f, 0.65f, 0.8f, 0.65f))
		{
			AudioEngine::Get().Play2D(click, 0);
			*gameState = 1;
			game->StartCampaign(level);
		}

		else if (gui::Button("Back", 0.5f, 0.2f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			menuState = State::MainMenu;
		}
		break;
	}
	/* Options */
	case State::Settings:
	{
		Renderer::Get().AddTexturedQuad(0.5f, 0.5f, 5.0f, 1.0f, 1.0f, bg_settings);

		gui::Slider(&(int&)windowWidth, 5, 8, 0.5f, 0.9f, 0.8f, 0.02f, { 0.2f, 0.2f, 0.2f });
		gui::Text("window width: " + numToString((int)windowWidth * 128) + " x " + numToString((int)windowWidth * 128), 0.5f, 0.85f, -6.0f, {0.5f, 0.5f, 0.5f});

		gui::CheckBox(&(bool&)showMinimap, 0.2f, 0.75f, 0.085f, { 0.2f, 0.2f, 0.2f });
		gui::Text("Always show mimimap", 0.6f, 0.75f, -6.0f, {0.5f, 0.5f, 0.5f});

		gui::Slider(&(int&)audioVolume, 0, 100, 0.5f, 0.6f, 0.8f, 0.02f, { 0.2f, 0.2f, 0.2f });
		gui::Text("Audio volume: " + numToString((int)audioVolume) + "%", 0.5f, 0.55f, -6.0f, {0.5f, 0.5f, 0.5f});

	    if (gui::Button("Back", 0.5f, 0.2f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			menuState = State::MainMenu;
		}
		if (gui::Button("Apply", 0.5f, 0.4f, 0.35f, 0.15f, button, button_hover))
		{
			AudioEngine::Get().Play2D(click, 0);
			//GraphicsDevice::Get().SetFullscreen(isFullscreen);
			Window::Get().ResizeWindow(windowWidth * 128, windowWidth * 128);
			AudioEngine::Get().SetMasterVolume(audioVolume / 100.0f);
			WriteSettings();
		}
		break;
	}
	}
}

void MainMenu::ReadSettings()
{
	std::ifstream outputFile("res/data/a.dat", std::ios::in | std::ios::binary);

	if (outputFile.fail())
	{
		LOG_CORE_ERROR("Could not open file res/data/a.dat");
		return;
	}

	outputFile.read((char*)(&windowWidth), sizeof(windowWidth));
	outputFile.read((char*)(&showMinimap), sizeof(showMinimap));
	outputFile.read((char*)(&audioVolume), sizeof(audioVolume));

	outputFile.close();

	return;
}

void MainMenu::WriteSettings()
{

	std::ofstream outputFile("res/data/a.dat", std::ios::binary | std::ios::out);
	outputFile.clear();

	outputFile.write((char*)(&windowWidth), sizeof(windowWidth));
	outputFile.write((char*)(&showMinimap), sizeof(showMinimap));
	outputFile.write((char*)(&audioVolume), sizeof(audioVolume));

	outputFile.close();
}