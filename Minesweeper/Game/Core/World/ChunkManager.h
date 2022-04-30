#pragma once
#include <unordered_map>
#include <thread>
#include <mutex>
#include "Graphics/Shader.h" 
#include "BlockManager.h"
#include "Graphics/Camera/Camera3D.h"
#include "TerrainGenerator/TerrainGenerator.h"
#include "Chunk.h"

struct hash_fn
{
    std::size_t operator() (const vec2i& node) const
    {
        std::size_t h1 = std::hash<int>()(node.x);
        std::size_t h2 = std::hash<int>()(node.y);

        return h1 ^ h2;
    }
};

class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();

	void Draw(Camera3D* camera);
    void LaunchChunkGenerationThread();
    /* Finds block in block coordinates */
    Block GetBlock(float x, float y, float z);
private:
    /* Returns chunk in chunk coordinates */
    Chunk* CreateChunk(vec2i index);
    /* Finds chunk in chunk coordinates else nullptr */
    Chunk* FindChunk(vec2i index);

    //Multithreaded
    void ChunkLoader();
	std::unordered_map<vec2i, Chunk*, hash_fn> chunks = std::unordered_map<vec2i, Chunk*, hash_fn>();
    std::mutex mutex;
    std::thread* chunkGenerationThread;
    bool isRunning = false;

    int32_t RENDER_DISTANCE = 1;
    std::vector<Vertex> meshData;

    TerrainGenerator terrainGenerator;
    Shader voxelShader;
    IndexBuffer indexBuffer;
};