#include "BlockManager.h"

TextureArray* BlockManager::textureArray = nullptr;

void BlockManager::Init()
{


	LoadBlockDataBase();
	LoadBlockTextures();
}

void BlockManager::LoadBlockDataBase()
{

}

void BlockManager::LoadBlockTextures()
{
	textureArray = new TextureArray(4);
	textureArray->AddTexture("res/Textures/dirt.png");
	textureArray->AddTexture("res/Textures/oakLogTop.png");
	textureArray->AddTexture("res/Textures/lampGreen.png");
	textureArray->AddTexture("res/Textures/sand.png");
	textureArray->Finish();
}