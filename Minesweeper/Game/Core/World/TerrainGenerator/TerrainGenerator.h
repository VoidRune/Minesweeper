#pragma once
#include "Core/World/Chunk.h"

class TerrainGenerator
{
public:
	TerrainGenerator() : seed(0) {};
	~TerrainGenerator() {};

	void GenerateTerrain(Chunk* chunk, vec2i offset);
	void SetSeed(int seed) { this->seed = seed; };

private:
	//Curently does nothing
	int seed;
};