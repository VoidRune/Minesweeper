#include "Game.h"
#include "Utility/Random.h"

Game::Game(int* gameState) :
	shader("res/Shaders/default.hlsl"),
	tiles(15),
	gameState(gameState)
{
	width = 0;
	height = 0;

	winGame = AudioEngine::Get().LoadData("res/Audio/win_game.ogg");
	loseGame = AudioEngine::Get().LoadData("res/Audio/lose_game.ogg");
	placeFlag = AudioEngine::Get().LoadData("res/Audio/place_flag.ogg");

	tiles.AddTexture("res/Textures/tile_blank.jpg");
	tiles.AddTexture("res/Textures/tile_1.jpg");
	tiles.AddTexture("res/Textures/tile_2.jpg");
	tiles.AddTexture("res/Textures/tile_3.jpg");
	tiles.AddTexture("res/Textures/tile_4.jpg");
	tiles.AddTexture("res/Textures/tile_5.jpg");
	tiles.AddTexture("res/Textures/tile_6.jpg");
	tiles.AddTexture("res/Textures/tile_7.jpg");
	tiles.AddTexture("res/Textures/tile_8.jpg");
	tiles.AddTexture("res/Textures/tile_bomb.jpg");
	tiles.AddTexture("res/Textures/tile_question.jpg");
	tiles.AddTexture("res/Textures/tile_covered.jpg");
	tiles.AddTexture("res/Textures/tile_flag.jpg");
	tiles.AddTexture("res/Textures/tile_bomb_exploded.jpg");
	tiles.AddTexture("res/Textures/tile_flag_false.jpg");
	tiles.Finish();
	/* Max 60 * 60 tiles */
	vertexBuffer.SetData(nullptr, 4 * 60 * 60, sizeof(Vertex2DArray));

	translate = XMMatrixTranspose(XMMatrixTranslation(1, 1, 1));
	transformConstBuffer.Set(&translate, 16 * sizeof(float), 2, 0);
	/*
	struct b
	{
		float tileOffsetX, tileOffsetY;
		float winX, winY;
		float scale, tilesInTexture;
		float pad1, pad2;
	}cb;
	cb.tileOffsetX = 0;
	cb.tileOffsetY = 0;
	cb.winX = Window::Get().GetDimensions().x;
	cb.winY = Window::Get().GetDimensions().y;
	cb.scale = scale;
	cb.tilesInTexture = tilesInTexture;

	csConstBuffer.Set(&cb, sizeof(cb), 7, 2);
	int* data1 = new int[64 * 64];
	int* data2 = new int[64 * 64];
	for (size_t i = 0; i < 64 * 64; i++)
	{
		data1[i] = i;
	}
	computeShader.Create("res/Shaders/mesh_compute.hlsl", 64 * 64, sizeof(int), 64 * 64, sizeof(int));
	computeShader.Dispatch(4, 1, 1, data1, data2);
	*/
}

Game::~Game()
{
	if (grid != nullptr) { delete grid; }
	if (gridOverlay != nullptr) { delete gridOverlay; }
	if (checkedGrid != nullptr) { delete checkedGrid; }
	if (difficultyScores != nullptr) { delete difficultyScores; }
	if (campaignScores != nullptr) { delete campaignScores; }
}

void Game::Start(int newBoardWidth, int newBoardHeight, float bombProbability, int32_t seed, int difficultyId)
{
	width = newBoardWidth;
	height = newBoardHeight;
	this->bombProbability = bombProbability;
	this->difficultyId = difficultyId;
	firstClick = true;
	canClick = true;
	isMenuOpen = false;
	gameHasEnded = false;
	isInCampaign = false;
	numOfBombs = 10;
	vertices.reserve(newBoardWidth * newBoardHeight * 4);

	if (grid != nullptr) { delete grid; }
	if (gridOverlay != nullptr) { delete gridOverlay; }
	if (checkedGrid != nullptr) { delete checkedGrid; }

	grid = new tile[(long)(newBoardWidth + 2) * (newBoardHeight + 2)];
	gridOverlay = new tile[(long)(newBoardWidth + 2) * (newBoardHeight + 2)];
	checkedGrid = new bool[(long)(newBoardWidth + 2) * (newBoardHeight + 2)];


	lastMousePos = { 1000, 1000 };
	scale = defaultScale;
	if (scale < 0.04f) { scale = 0.04f; }
	else if (scale > 0.2f) { scale = 0.2f; }
	if (1.0f / scale > width || 1.0f / scale > height) { scale = (width < height) ? 1.0f / width : 1.0f / height; }

	float s = (1.0f / scale); /* Tiles in current frame */
	tileOffset.x = s - 5.0f + (width - s);
	tileOffset.y = s - 5.0f + (height - s);

	translate = XMMatrixTranslation(tileOffset.x + 5.0f, tileOffset.y + 5.0f, 1);
	translate = XMMatrixMultiplyTranspose(translate, XMMatrixScaling(scale, scale, 1));
	transformConstBuffer.Load(&translate, sizeof(translate));

	ClearBoard();

#ifndef EDIT_LEVELS
	GenerateTiles(bombProbability, seed);
#endif

	UpdateBoard();
}

void Game::StartCampaign(int levelID)
{
	firstClick = true;
	canClick = true;
	isMenuOpen = false;
	gameHasEnded = false;
	isInCampaign = true;
	campaignLevel = levelID;
	numOfBombs = 10;
	if (!ReadCampaignLevel("res/data/level" + numToString(levelID) + ".dat")) { *gameState = 0; return; }
	vertices.reserve(width * height * 4);
	
	lastMousePos = { 1000, 1000 };
	scale = defaultScale;
	if (scale < 0.04f) { scale = 0.04f; }
	else if (scale > 0.2f) { scale = 0.2f; }
	if (1.0f / scale > width || 1.0f / scale > height) { scale = (width < height) ? 1.0f / width : 1.0f / height; }

	float s = (1.0f / scale); /* Tiles in current frame */
	tileOffset.x = s - 5.0f + (width - s);
	tileOffset.y = s - 5.0f + (height - s);

	translate = XMMatrixTranslation(tileOffset.x + 5.0f, tileOffset.y + 5.0f, 1);
	translate = XMMatrixMultiplyTranspose(translate, XMMatrixScaling(scale, scale, 1));
	transformConstBuffer.Load(&translate, sizeof(translate));

	CountBombs();
	UpdateBoard();
}

void Game::Update()
{
	timer.UpdateDt();

	bool moved = false;
	if (Input::MouseDeltaPosition() != 0)
	{
		scale += Input::MouseDeltaPosition() * 0.01f;
		if (scale < 0.04f) { scale = 0.04f; }
		else if (scale > 0.2f) { scale = 0.2f; }
		if (1.0f / scale > width || 1.0f / scale > height) { scale = (width < height) ? 1.0f / width : 1.0f / height; }
		moved = true;
	}

	float s = (1.0f / scale); /* Tiles in current frame */

	bool clicked = false;
	bool renderMap = false;
	if (Input::IsLeftMouseKeyPressed())
	{
		lastMousePos = Input::MousePosition();
	}
	else if (Input::IsLeftMouseKeyReleased())
	{
		clicked = distanceNRoot(lastMousePos, Input::MousePosition()) < 225;
	}
	if (Input::IsLeftMouseKeyDown() && distanceNRoot(lastMousePos, Input::MousePosition()) > 225)
	{
		lastMousePos = { -20, -20 };
		tileOffset.x -= Input::MousePosDifference().x / (Window::Get().GetDimensions().x / (s * 2.0f));
		tileOffset.y += Input::MousePosDifference().y / (Window::Get().GetDimensions().y / (s * 2.0f));
		renderMap = true;
		moved = true;
	}

	if (tileOffset.x < s - 5.0f) { tileOffset.x = s - 5.0f; }
	if (tileOffset.y < s - 5.0f) { tileOffset.y = s - 5.0f; }
	if (tileOffset.x > s - 5.0f + (width - s) * 2.0f) { tileOffset.x = s - 5.0f + (width - s) * 2.0f; }
	if (tileOffset.y > s - 5.0f + (height - s) * 2.0f) { tileOffset.y = s - 5.0f + (height - s) * 2.0f; }

	if (moved)
	{
		translate = XMMatrixTranslation(tileOffset.x + 5.0f, tileOffset.y + 5.0f, 1);
		translate = XMMatrixMultiplyTranspose(translate, XMMatrixScaling(scale, scale, 1));
		transformConstBuffer.Load(&translate, sizeof(translate));
	}

	if (*showMinimap || renderMap)
	{
		float k = (width > height) ? width : height;
		float w = width / k * 0.2f;
		float h = height / k * 0.2f;
		float l = (s / k) * ((w > h) ? w : h);

		Renderer::Get().Add2DQuad(0.85f, 0.85f, -4.0f, w, h, { 0.6, 0.8, 1.0, 0.3f + renderMap * 0.5f });
		Renderer::Get().Add2DQuad(0.85f + (((tileOffset.x + 5) / width) - 1) * ((width < height) ? ((float)width / (float)height) * 0.1f : 0.1f), 0.85f + (((tileOffset.y + 5) / height) - 1) * ((height < width) ? ((float)height / (float)width) * 0.1f : 0.1f), -4.2f, l, l, { 0.4, 0.4, 0.4, 0.5f + renderMap * 0.5f });
	}


#ifdef EDIT_LEVELS
	LevelEditor();
	//if (needsUpdating)
	//{
	//	if (inOverlayMode) { UpdateBoard(); }
	//	else { UpdateEditingTiles(); }
	//	needsUpdating = false;
	//}
	return;
#endif

	int x = (Input::MousePosition().x / Window::Get().GetDimensions().x) * (1.0f / scale) + tileOffset.x * 0.5f + (5.0f - (1.0f / scale)) * 0.5f;
	int y = (1.0f - (Input::MousePosition().y / Window::Get().GetDimensions().y)) * (1.0f / scale) + tileOffset.y * 0.5f + (5.0f - (1.0f / scale)) * 0.5f;

	if (x < 0) { x = 0; }
	else if (x >= width) { x = width - 1; }
	if (y < 0) { y = 0; }
	else if (y >= height) { y = height - 1; }

	if (Input::IsMouseKeyPressed(1) && canClick)
	{

		/* Place flag */
		if (getOverlay(x, y) == tile::covered) { setOverlay(x, y, tile::flag); AudioEngine::Get().Play2D(placeFlag, 0); }
		else if(getOverlay(x, y) == tile::flag)	setOverlay(x, y, tile::covered);
		if (firstClick)
		{
			firstClick = !isInCampaign;
			timer.reset_time();
		}
		CheckForWin();
		UpdateBoard();
	}
	else if (clicked && canClick)
	{

		if (firstClick)
		{
			firstClick = false;
			if (!isInCampaign)
			{
				/* delete bombs in 9x9 area when clicked */
				setTile(x, y, tile::blank);
				setTile(x + 1, y, tile::blank);
				setTile(x + 1, y + 1, tile::blank);
				setTile(x, y + 1, tile::blank);
				setTile(x - 1, y + 1, tile::blank);
				setTile(x - 1, y, tile::blank);
				setTile(x - 1, y - 1, tile::blank);
				setTile(x, y - 1, tile::blank);
				setTile(x + 1, y - 1, tile::blank);
				GenerateNumbers();
				CountBombs();
			}
			timer.reset_time();
		}

		tile currentTile = getTile(x, y);
		tile currentOverlay = getOverlay(x, y);
		if (currentOverlay < tile::bomb)  //reveals the rest when clicked on number if there are enough flags around
		{
			int n = 0;

			if (getOverlay(x + 1, y) == tile::flag) n++;
			if (getOverlay(x, y + 1) == tile::flag) n++;
			if (getOverlay(x - 1, y) == tile::flag) n++;
			if (getOverlay(x, y - 1) == tile::flag) n++;
			if (getOverlay(x + 1, y + 1) == tile::flag) n++;
			if (getOverlay(x - 1, y - 1) == tile::flag) n++;
			if (getOverlay(x + 1, y - 1) == tile::flag) n++;
			if (getOverlay(x - 1, y + 1) == tile::flag) n++;

			if ((int)currentOverlay <= n)
			{
				if (getOverlay(x + 1, y) == tile::covered)  setOverlay(x + 1, y, getTile(x + 1, y));  CheckTiles(x + 1, y);
				if (getOverlay(x, y + 1) == tile::covered)  setOverlay(x, y + 1, getTile(x, y + 1));  CheckTiles(x, y + 1);
				if (getOverlay(x - 1, y) == tile::covered)  setOverlay(x - 1, y, getTile(x - 1, y));  CheckTiles(x - 1, y);
				if (getOverlay(x, y - 1) == tile::covered)  setOverlay(x, y - 1, getTile(x, y - 1));  CheckTiles(x, y - 1);
				if (getOverlay(x + 1, y + 1) == tile::covered)  setOverlay(x + 1, y + 1, getTile(x + 1, y + 1));  CheckTiles(x + 1, y + 1);
				if (getOverlay(x - 1, y - 1) == tile::covered)  setOverlay(x - 1, y - 1, getTile(x - 1, y - 1));  CheckTiles(x - 1, y - 1);
				if (getOverlay(x + 1, y - 1) == tile::covered)  setOverlay(x + 1, y - 1, getTile(x + 1, y - 1));  CheckTiles(x + 1, y - 1);
				if (getOverlay(x - 1, y + 1) == tile::covered)  setOverlay(x - 1, y + 1, getTile(x - 1, y + 1));  CheckTiles(x - 1, y + 1);

			}
		}
		else if (currentOverlay != tile::flag)
		{

			setOverlay(x, y, currentTile);
			CheckTiles(x, y);
		}
	
		CheckForWin();
		UpdateBoard();
	}

	Draw();
	UpdateMenu();
}

void Game::CheckForWin()
{
	int score = 0;
	int flagScore = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tile currOverlay = getOverlay(x, y);
			tile currTile = getTile(x, y);

			if (currOverlay == tile::bomb)
			{
				timeTaken = timer.elapsed_sec();
				RevealBombs();
				AudioEngine::Get().Play2D(loseGame, 0);
				canClick = false;
				gameHasEnded = true;
				isMenuOpen = true;
				/* Exits both loops */
				x = width;
				y = height;
			};
			if (currOverlay == tile::flag && currTile == tile::bomb) { flagScore++; }
			if (currOverlay == tile::flag && currTile == tile::bomb || currOverlay == tile::covered && currTile == tile::bomb) { score++; }
			if (currOverlay == tile::flag && currTile < tile::bomb) { flagScore--; }
			if (currOverlay == tile::covered && currTile < tile::bomb || currOverlay == tile::flag && currTile < tile::bomb) { score--; }
		}
	}
	if (score == numOfBombs || flagScore == numOfBombs)
	{
		timeTaken = timer.elapsed_sec();
		AudioEngine::Get().Play2D(winGame, 0);
		canClick = false;
		gameHasEnded = true;
		isMenuOpen = true;
		float t = ((isInCampaign) ? campaignScores[campaignLevel] : difficultyScores[difficultyId]);
		if (isInCampaign && *currCampaignLevel == campaignLevel && *currCampaignLevel <= 30){ *currCampaignLevel = campaignLevel + 1; }

		if (timeTaken < t || t == -1)
		{
			if (isInCampaign)
			{
				campaignScores[campaignLevel] = timeTaken;
			}
			else
			{
				difficultyScores[difficultyId] = timeTaken;
			}
			WriteBestScores();
		}
	}
}

void Game::RevealBombs()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tile currOverlay = getOverlay(x, y);
			tile currTile = getTile(x, y);

			if (currOverlay == tile::bomb) { setOverlay(x, y, tile::bomb_exploded); }
			else if (!isInCampaign)
			{
				if (currTile == tile::bomb && currOverlay != tile::flag) { setOverlay(x, y, tile::bomb); }
				else if (currTile < tile::bomb && currOverlay == tile::flag) { setOverlay(x, y, tile::flag_false); }
			}
		}
	}
}

void Game::GenerateTiles(float bombProbability, int32_t seed)
{
	Random::seed = (seed == 0) ? timer.elapsed_micro() : seed;
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			int t = Random::Chance(bombProbability);
			if (t == 1)
			{
				setTile(x, y, tile::bomb);
			}
		}
	}
}
void Game::GenerateNumbers()
{
	numOfBombs = 0;
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			if (getTile(x, y) == tile::bomb || getTile(x, y) == tile::question)
			{
				setChecked(x, y, true);
				continue;
			}
			int i = 0;
			if (getTile(x + 1, y) == tile::bomb) { i++; }
			if (getTile(x + 1, y + 1) == tile::bomb) { i++; }
			if (getTile(x, y + 1) == tile::bomb) { i++; }
			if (getTile(x - 1, y + 1) == tile::bomb) { i++; }

			if (getTile(x - 1, y) == tile::bomb) { i++; }
			if (getTile(x - 1, y - 1) == tile::bomb) { i++; }
			if (getTile(x, y - 1) == tile::bomb) { i++; }
			if (getTile(x + 1, y - 1) == tile::bomb) { i++; }
			setTile(x, y, (tile)i);
		}
	}
}

void Game::UpdateBoard()
{
	vertices.clear();

	float winW = Window::Get().GetDimensions().x;
	float winH = Window::Get().GetDimensions().y;

	for (float y = 0; y < height; y++)
	{
		for (float x = 0; x < width; x++)
		{
			int tile = (int)getOverlay(x, y);
			vertices.emplace_back(x, y, 5,			0 , 0, tile);
			vertices.emplace_back(x, y + 1, 5,		0 , 1, tile);
			vertices.emplace_back(x + 1, y + 1, 5,	1 , 1, tile);
			vertices.emplace_back(x + 1, y, 5,		1 , 0, tile);
		}
	}
	vertexBuffer.SetData(vertices.data(), vertices.size(), sizeof(Vertex2DArray));
	numOfVerticesToDraw = vertices.size() * 1.5f;
}
void Game::UpdateEditingTiles()
{
	vertices.clear();

	float winW = Window::Get().GetDimensions().x;
	float winH = Window::Get().GetDimensions().y;

	for (float y = 0; y < height; y++)
	{
		for (float x = 0; x < width; x++)
		{
			int tile = (int)getTile(x, y);
			vertices.emplace_back(x, y, 5, 0, 0, tile);
			vertices.emplace_back(x, y + 1, 5, 0, 1, tile);
			vertices.emplace_back(x + 1, y + 1, 5, 1, 1, tile);
			vertices.emplace_back(x + 1, y, 5, 1, 0, tile);
		}
	}
	vertexBuffer.SetData(vertices.data(), vertices.size(), sizeof(Vertex2DArray));
	numOfVerticesToDraw = vertices.size() * 1.5f;
}
void Game::ClearBoard()
{
	for (size_t i = 0; i < (width + 2) * (height + 2); i++)
	{
		grid[i] = tile::blank;
		gridOverlay[i] = tile::covered;
	}
	for (size_t y = 0; y < height + 2; y++)
	{
		for (size_t x = 0; x < width + 2; x++)
		{
			if (x == 0 || y == 0 || x == (width + 1) || y == (height + 1))
				checkedGrid[to1DArray(x, y)] = true;
			else
				checkedGrid[to1DArray(x, y)] = false;
		}
	}
}

void Game::CountBombs()
{
	numOfBombs = 0;
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			if (getTile(x, y) == tile::bomb) { numOfBombs++; }
		}
	}
}

void Game::setOverlay(int x, int y, tile id)
{
	gridOverlay[to1DArray(x + 1, y + 1)] = id;
}

Game::tile Game::getOverlay(int x, int y)
{
	return (tile)gridOverlay[to1DArray(x + 1, y + 1)];
}

void Game::setTile(int x, int y, tile id)
{
	grid[to1DArray(x + 1, y + 1)] = id;
}

Game::tile Game::getTile(int x, int y)
{
	return (tile)grid[to1DArray(x + 1, y + 1)];
}

void Game::setChecked(int x, int y, bool id)
{
	checkedGrid[to1DArray(x + 1, y + 1)] = id;
}

uint32_t Game::to1DArray(uint8_t x, uint8_t y)
{
	return x + y * (width + 2);
}

void Game::CheckTiles(uint8_t x, uint8_t y)
{
	x += 1;
	y += 1;
	//-----checks if there are any blank spots around and then checks around them etc-----//
	if (gridOverlay[to1DArray(x, y)] == tile::blank && checkedGrid[to1DArray(x, y)] == false)
	{
		checkedGrid[to1DArray(x, y)] = true;
		gridOverlay[to1DArray(x + 1, y)] = grid[to1DArray(x + 1, y)];
		gridOverlay[to1DArray(x, y + 1)] = grid[to1DArray(x, y + 1)];
		gridOverlay[to1DArray(x - 1, y)] = grid[to1DArray(x - 1, y)];
		gridOverlay[to1DArray(x, y - 1)] = grid[to1DArray(x, y - 1)];
		gridOverlay[to1DArray(x + 1, y + 1)] = grid[to1DArray(x + 1, y + 1)];
		gridOverlay[to1DArray(x - 1, y - 1)] = grid[to1DArray(x - 1, y - 1)];
		gridOverlay[to1DArray(x + 1, y - 1)] = grid[to1DArray(x + 1, y - 1)];
		gridOverlay[to1DArray(x - 1, y + 1)] = grid[to1DArray(x - 1, y + 1)];
		/* if (checkedGrid[to1DArray(x + 1, y)] == false) */ CheckTiles(x, y - 1);
		/* if (checkedGrid[to1DArray(x, y + 1)] == false) */ CheckTiles(x - 1, y);
		/* if (checkedGrid[to1DArray(x - 1, y)] == false) */ CheckTiles(x - 2, y - 1);
		/* if (checkedGrid[to1DArray(x, y - 1)] == false) */ CheckTiles(x - 1, y - 2);
		/* if (checkedGrid[to1DArray(x + 1, y + 1)] == false) */ CheckTiles(x, y);
		/* if (checkedGrid[to1DArray(x - 1, y - 1)] == false) */ CheckTiles(x - 2, y - 2);
		/* if (checkedGrid[to1DArray(x + 1, y - 1)] == false) */ CheckTiles(x, y - 2);
		/* if (checkedGrid[to1DArray(x - 1, y + 1)] == false) */ CheckTiles(x - 2, y);
	}
}

void Game::UpdateMenu()
{
	if (Input::IsKeyPressed(27))
	{
		isMenuOpen = !isMenuOpen;
		if (!gameHasEnded)
			canClick = !canClick;
	}
	if (isMenuOpen)
	{
		gui::Sprite(0.5f, 0.5f, -2.0f, 0.6f, 0.8f, tex_menu);
		if (gui::Button("Restart", 0.5f, 0.7f, 0.35f, 0.15f, tex_button, tex_button_hover))
		{
			AudioEngine::Get().Play2D(ResourceManager::Get().sounds.at("click"), 0);
			if (isInCampaign) { StartCampaign(campaignLevel); }
			else { Start(width, height, bombProbability, 0, difficultyId); }
		}
		if (isInCampaign && (*currCampaignLevel > campaignLevel) && gui::Button("->", 0.73f, 0.7f, 0.09f, 0.09f, { 0.1, 0.1, 0.1 }))
		{
			AudioEngine::Get().Play2D(ResourceManager::Get().sounds.at("click"), 0);
			StartCampaign(campaignLevel + 1);
		}
		if (gui::Button("Back", 0.5f, 0.5f, 0.35f, 0.15f, tex_button, tex_button_hover))
		{
			AudioEngine::Get().Play2D(ResourceManager::Get().sounds.at("click"), 0);
			*gameState = 0;
		}
		if (((isInCampaign) ? campaignScores[campaignLevel] : difficultyScores[difficultyId]) != -1 && difficultyId != 4)
		{
			gui::Text("Best time: " + timeAsString((isInCampaign) ? campaignScores[campaignLevel] : difficultyScores[difficultyId]), 0.5f, 0.2f, -6.0f, { 0.0f, 0.0f, 0.0f });
		}
		if (gameHasEnded)
		{
			gui::Text("Time: " + timeAsString(timeTaken), 0.5f, 0.3f, -6.0f, { 0.0f, 0.0f, 0.0f });
		}
	}
}

void Game::setTilesWidth(int newscale)
{
	defaultScale = newscale;
}

void Game::Draw()
{
	tiles.Bind(0);
	shader.Bind();
	ResourceManager::Get().BindQuadIndexBuffer();
	vertexBuffer.Bind();
	GraphicsDevice::Get().context->DrawIndexed(numOfVerticesToDraw, 0, 0);
}

void Game::SyncOverlay()
{
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			if (getTile(x, y) == tile::bomb && getOverlay(x, y) < tile::covered) { setOverlay(x, y, tile::covered); }
			else if (getOverlay(x, y) < tile::covered) { setOverlay(x, y, getTile(x, y)); }
		}
	}
}

void Game::LevelEditor()
{
	if (Input::IsKeyPressed('Q'))
	{
		inOverlayMode = !inOverlayMode;
		SyncOverlay();
		if (inOverlayMode) { UpdateBoard(); }
		else { UpdateEditingTiles(); }
	}
	if (Input::IsKeyPressed('K'))
	{
		/* Save level */
		GenerateNumbers();
		SyncOverlay();
#ifdef EDIT_LEVELS
		WriteCampaignLevel("res/data/level" + numToString(CURRENT_LEVEL_ID));
#else
		//Saved as number 0 (will save, but not used in game)
		WriteCampaignLevel("res/data/level" + numToString(0));
#endif
		AudioEngine::Get().Play2D(ResourceManager::Get().sounds.at("click"), 0);
	}

	if (Input::IsMouseKeyPressed(1) && canClick)
	{
		int x = (Input::MousePosition().x / Window::Get().GetDimensions().x) * (1.0f / scale) + tileOffset.x * 0.5f + (5.0f - (1.0f / scale)) * 0.5f;
		int y = (1.0f - (Input::MousePosition().y / Window::Get().GetDimensions().y)) * (1.0f / scale) + tileOffset.y * 0.5f + (5.0f - (1.0f / scale)) * 0.5f;

		if (inOverlayMode)
		{
			/* Place flag */
			if (getOverlay(x, y) == tile::covered) { setOverlay(x, y, tile::flag); }
			else if (getOverlay(x, y) == tile::flag)	setOverlay(x, y, tile::covered);
			UpdateBoard();
		}
		else
		{
			if (getTile(x, y) < tile::bomb) { setTile(x, y, tile::bomb); }
			else setTile(x, y, tile::blank);
			GenerateNumbers();
			UpdateEditingTiles();
		}
	}
	if (Input::IsMouseKeyPressed(0) && canClick)
	{
		int x = (Input::MousePosition().x / Window::Get().GetDimensions().x) * (1.0f / scale) + tileOffset.x * 0.5f + (5.0f - (1.0f / scale)) * 0.5f;
		int y = (1.0f - (Input::MousePosition().y / Window::Get().GetDimensions().y)) * (1.0f / scale) + tileOffset.y * 0.5f + (5.0f - (1.0f / scale)) * 0.5f;

		if (inOverlayMode)
		{
			if (getOverlay(x, y) == tile::covered && getTile(x, y) != tile::bomb) { setOverlay(x, y, getTile(x, y)); }
			else setOverlay(x, y, tile::covered);
			UpdateBoard();
		}
		else
		{
			if (getTile(x, y) != tile::bomb && getTile(x, y) != tile::question) { setTile(x, y, tile::question); }
			else if (getTile(x, y) == tile::question)	setTile(x, y, tile::blank);
			GenerateNumbers();
			UpdateEditingTiles();
		}

	}

	Draw();
	UpdateMenu();
}

void Game::LoadBestScores(uint32_t* currLvl, bool* showMinimap)
{
	this->showMinimap = showMinimap;
	tex_button = ResourceManager::Get().batchedTextures.at("uiTex").get_coords("res/Textures/button.png");
	tex_button_hover = ResourceManager::Get().batchedTextures.at("uiTex").get_coords("res/Textures/button_hover.png");
	tex_menu = ResourceManager::Get().batchedTextures.at("uiTex").get_coords("res/Textures/Menu.png");

	std::ifstream outputFile("res/data/b.dat", std::ios::in | std::ios::binary);

	currCampaignLevel = currLvl;
	if (difficultyScores != nullptr) { delete difficultyScores; }
	if (campaignScores != nullptr) { delete campaignScores; }
	difficultyScores = new float[5];
	campaignScores = new float[30];


	if (outputFile.fail())
	{
		LOG_CORE_ERROR("Could not open file res/data/b.dat");
		*currCampaignLevel = 1;
		for (size_t i = 0; i < 5; i++)
		{
			difficultyScores[i] = -1;
		}
		for (size_t i = 0; i < 30; i++)
		{
			campaignScores[i] = -1;
		}
		outputFile.close();
		return;
	}

	outputFile.read((char*)(currCampaignLevel), sizeof(uint32_t));
	outputFile.read((char*)(difficultyScores), 5 * sizeof(float));
	outputFile.read((char*)(campaignScores), 30 * sizeof(float));
	outputFile.close();
}

void Game::WriteBestScores()
{
	std::ofstream outputFile("res/data/b.dat", std::ios::binary | std::ios::out);
	outputFile.clear();
	outputFile.write((char*)(currCampaignLevel), sizeof(uint32_t));
	outputFile.write((char*)(difficultyScores), 5 * sizeof(float));
	outputFile.write((char*)(campaignScores), 30 * sizeof(float));
	outputFile.close();
}

void Game::WriteCampaignLevel(std::string path)
{
	std::string subname = path.substr(0, path.find_last_of('.'));

	std::ofstream outputFile(subname + ".dat", std::ios::binary | std::ios::out);
	outputFile.clear();

	outputFile.write((char*)(&width ), sizeof(uint32_t));
	outputFile.write((char*)(&height), sizeof(uint32_t));

	outputFile.write((char*)(grid), (width + 2) * (height + 2) * sizeof(uint32_t));
	outputFile.write((char*)(gridOverlay), (width + 2) * (height + 2) * sizeof(uint32_t));

	outputFile.close();
}

bool  Game::ReadCampaignLevel(std::string path)
{
	std::ifstream outputFile(path, std::ios::in | std::ios::binary);

	if (outputFile.fail()) 
	{ 
		LOG_CORE_ERROR("Could not open file %s", path.c_str());
		outputFile.close();
		return false;
	}

	outputFile.read((char*)(&width ), sizeof(uint32_t));
	outputFile.read((char*)(&height), sizeof(uint32_t));

	if (width > 512 || height > 512) { return false; }

	if (grid != nullptr) { delete grid; }
	if (gridOverlay != nullptr) { delete gridOverlay; }
	if (checkedGrid != nullptr) { delete checkedGrid; }
	grid = new tile[(width + 2) * (height + 2)];
	gridOverlay = new tile[(width + 2) * (height + 2)];
	checkedGrid = new bool[(width + 2) * (height + 2)];

	ClearBoard();

	outputFile.read((char*)grid, (width + 2) * (height + 2) * sizeof(uint32_t));
	outputFile.read((char*)gridOverlay, (width + 2) * (height + 2) * sizeof(uint32_t));

	outputFile.close();

	return true;
}