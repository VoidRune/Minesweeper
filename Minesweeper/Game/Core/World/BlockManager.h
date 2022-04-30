#pragma once
#include "Graphics/Texture/Texture.h"

class BlockManager
{
public:
	static void Init();

	static TextureArray* textureArray;
private:
	static void LoadBlockDataBase();
	static void LoadBlockTextures();
};