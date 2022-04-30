#include "TerrainGenerator.h"
#include "Utility/SimplexNoise.h"

void TerrainGenerator::GenerateTerrain(Chunk* chunk, vec2i offset)
{
	uint32_t* heightMap = new uint32_t[CHUNK_SLICE];


	for (int z = 0; z < CHUNK_WIDTH; z++)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			uint32_t height = std::floor(SimplexNoise::noise((x + (int)offset.x * (int)CHUNK_WIDTH) / 64.0f, (z + (int)offset.y * (int)CHUNK_WIDTH) / 64.0f) * 16 + 32);
			heightMap[chunk->to1DArray(x, 0, z)] = height;
			if (height > chunk->highestBlock) { chunk->highestBlock = height; }
		}
	}

	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				if (y < heightMap[chunk->to1DArray(x, 0, z)])
				{
					chunk->blockData[chunk->to1DArray(x, y, z)] = { 1 };
				}
				else
				{
					chunk->blockData[chunk->to1DArray(x, y, z)] = { 0 };
				}
			}
		}
	}
	chunk->hasBlockData = true;
}