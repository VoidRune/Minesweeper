#pragma once
#include "Window/Window.h"
#include "Graphics/GraphicsDevice.h"
#include "Audio/AudioEngine.h"
#include "Core/Network/Networking.h"
#include "Graphics/Renderer.h"

#include "OptionsMenu.h"
#include "Time.h"
#include "Entity/EntityPlayer.h"
#include "Core/World/ChunkManager.h"
#include "Core/SkyBox.h"
#include "Graphics/ResourceManager.h"
#include "ImGui/ImGuiImpl.h"

/* define EDIT_LEVELS for editing campaign levels */
#ifdef _DEBUG
	//#define EDIT_LEVELS
	#define CURRENT_LEVEL_ID 0
#endif

class Game
{
public:
	Game(int* gameState);
	~Game();

	void Start(int tileWidth, int tileHeight, float bombProbability, int32_t seed, int difficultyId);
	void StartCampaign(int levelID);
	void Update();
	void Draw();
	void setTilesWidth(int newTileWidth);

	void LevelEditor();
	void LoadBestScores(uint32_t* currLvl, bool* showMinimap);
	void WriteBestScores();
private:
	void UpdateBoard();
	void GenerateTiles(float bombProbability, int32_t seed);
	void GenerateNumbers();
	void ClearBoard();
	void CheckTiles(uint8_t x, uint8_t y);
	void CheckForWin();
	void RevealBombs();
	void CountBombs();

	void UpdateMenu();

	void WriteCampaignLevel(std::string path);
	bool ReadCampaignLevel(std::string path);

	enum class tile : uint32_t
	{
		blank = 0,
		_1,
		_3,
		_2,
		_4,
		_5,
		_6,
		_7,
		_8,
		bomb,
		question,
		covered,
		flag,
		bomb_exploded,
		flag_false,

		COUNT
	};
	void setOverlay(int x, int y, tile id);
	tile getOverlay(int x, int y);
	void setTile(int x, int y, tile id);
	tile getTile(int x, int y);
	void setChecked(int x, int y, bool id);
	uint32_t to1DArray(uint8_t x, uint8_t y);

	/* Game */
	Time timer;
	float timeTaken;

	uint32_t width;
	uint32_t height;

	tile* grid; /* Where the bombs are */
	tile* gridOverlay; /* Overlay */
	bool* checkedGrid;
	uint32_t numOfBombs;
	float bombProbability;

	/* -------- */
	bool firstClick;
	bool canClick;
	bool gameHasEnded;
	bool isMenuOpen;
	bool isInCampaign;
	bool* showMinimap;
	int difficultyId;
	uint32_t campaignLevel;
	uint32_t* currCampaignLevel;
	float* difficultyScores; /* size: 5 */
	float* campaignScores;  /* size: 30 */

	/* -------- */
	/* Only when editing levels */
	bool inOverlayMode = true;
	void UpdateEditingTiles();
	void SyncOverlay();
	/* -------- */

	float tilesInTexture = (float)tile::COUNT;
	vec2f tileOffset = { 0, 0 };
	float scale;
	float defaultScale = 0.1f;
	vec2f lastMousePos;

	Shader shader;
	//ComputeShader computeShader;
	//ConstantBuffer csConstBuffer;
	ConstantBuffer transformConstBuffer;
	XMMATRIX translate;
	TextureArray tiles;
	VertexBuffer vertexBuffer;
	std::vector<Vertex2DArray> vertices;
	uint32_t numOfVerticesToDraw;
	vec4f tex_button, tex_button_hover, tex_menu;

	Sound winGame;
	Sound loseGame;
	Sound placeFlag;
	
	int* gameState;
};