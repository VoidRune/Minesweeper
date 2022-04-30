#include "ChunkManager.h"
#include "Time.h"
ChunkManager::ChunkManager():
	voxelShader("res/Shaders/voxel.hlsl")
{
	chunks.reserve((2 * RENDER_DISTANCE + 1) * (2 * RENDER_DISTANCE + 1));

	meshData.reserve(SEGMENT_WIDTH * SEGMENT_WIDTH * SEGMENT_HEIGHT * 24);
	BlockManager::Init();

	/* Max 6000 quads drawn per chunk segment */
	int maxIndices = 6 * 6000;
	uint32_t* indices = new uint32_t[maxIndices];
	uint32_t offset = 0;
	for (size_t i = 0; i < maxIndices; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	indexBuffer.SetData(indices, maxIndices);
}

ChunkManager::~ChunkManager()
{
	if (chunkGenerationThread != nullptr) {
		isRunning = false;

		if (chunkGenerationThread->joinable())
			chunkGenerationThread->join();

		delete chunkGenerationThread;
	}
}

void ChunkManager::LaunchChunkGenerationThread()
{
	isRunning = true;
	if (chunkGenerationThread == nullptr)
		chunkGenerationThread = new std::thread(&ChunkManager::ChunkLoader, this);
}

void ChunkManager::ChunkLoader()
{
	std::unique_lock<std::mutex> lock(mutex);
	lock.unlock();
	
	vec2i currChunk = { 0, 0 };
	vec2i lastChunk = { 1, 1 };

	while (isRunning)
	{
		if (currChunk != lastChunk)
		{
			for (int i = 0; i <= RENDER_DISTANCE; i++)
			{
				int minX = currChunk.x - i;
				int maxX = currChunk.x + i;
				int minZ = currChunk.y - i;
				int maxZ = currChunk.y + i;
				for (int z = minZ; z <= maxZ; z++)
				{
					for (int x = minX; x <= maxX; x++)
					{
						if (z != minZ && z != maxZ && x != minX && x != maxX)
						{
							x += 2 * maxX - 2; continue;
						}

						vec2i index = { x, z };
						// Thread Safety
						Chunk* chunk = CreateChunk(index);

						if (!chunk->hasBlockData)
						{
							lock.lock();
							terrainGenerator.GenerateTerrain(chunk, index);
							lock.unlock();
						}

						std::this_thread::sleep_for(std::chrono::microseconds(50));
					}
				}
				int minI = minZ + 1;
				int maxI = maxZ - 1;
				int minJ = minX + 1;
				int maxJ = maxX - 1;
				for (int i = minI; i <= maxI; i++)
				{
					for (int j = minJ; j <= maxJ; j++)
					{
						if (i != minI && i != maxI && j != minJ && j != maxJ)
						{
							j += 2 * maxJ - 2; continue;
						}
						vec2i index = { j, i };
						// Thread Safety
						Chunk* chunk = FindChunk(index);
						lock.lock();
						chunk->CreateNeighbourData(index,
							FindChunk({ index.x - 1, index.y }),
							FindChunk({ index.x, index.y + 1 }),
							FindChunk({ index.x + 1, index.y }),
							FindChunk({ index.x, index.y - 1 })
						);
						chunk->CreateMeshData(index);
						lock.unlock();
					}
				}
				if (i == RENDER_DISTANCE)
				{
					for (int i = minZ; i <= maxZ; i++)
					{
						for (int j = minX; j <= maxX; j++)
						{
							if (i != minZ && i != maxZ && j != minX && j != maxX)
							{
								j += 2 * maxX - 2; continue;
							}
							vec2i index = { j, i };
							// Thread Safety
							Chunk* chunk = FindChunk(index);
							lock.lock();
							chunk->CreateNeighbourData(index,
								FindChunk({ index.x - 1, index.y }),
								FindChunk({ index.x, index.y + 1 }),
								FindChunk({ index.x + 1, index.y }),
								FindChunk({ index.x, index.y - 1 })
							);
							chunk->CreateMeshData(index);
							lock.unlock();
						}
					}
				}
			}
			lastChunk = currChunk;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::microseconds(50));
		}
	}
}

Chunk* ChunkManager::CreateChunk(vec2i index)
{
	Chunk* chunk = FindChunk(index);
	if (chunk != nullptr) { return chunk; }

	chunk = new Chunk(index, &meshData);
	std::unique_lock<std::mutex> lock(mutex);
	chunks.emplace(index, chunk);

	return chunk;
}

Chunk* ChunkManager::FindChunk(vec2i index)
{
	auto iter = chunks.find(index);
	return (iter != chunks.end()) ? iter->second : nullptr;
}

Block ChunkManager::GetBlock(float x, float y, float z)
{
	if (y < 0 || y > CHUNK_HEIGHT) { return { 0 }; }
	Chunk* chunk = FindChunk({(int)std::floor(x / CHUNK_WIDTH), (int)std::floor(z / CHUNK_WIDTH)});
	//LOG_INFO("Chunk x: %d, z: %d", (int)std::floor(x / CHUNK_WIDTH), (int)std::floor(z / CHUNK_WIDTH));
	//LOG_INFO("Block x: %d, y: %d, z: %d", (int)x - chunk->chunkOffset.x * CHUNK_WIDTH, (int)y, (int)z - chunk->chunkOffset.y * CHUNK_WIDTH);
	if (chunk != nullptr) { return chunk->GetBlock(x - chunk->chunkOffset.x * CHUNK_WIDTH, y, z - chunk->chunkOffset.y * CHUNK_WIDTH); }
	return { 0 };
}

void ChunkManager::Draw(Camera3D* camera)
{
	indexBuffer.Bind();
	BlockManager::textureArray->Bind(0);
	voxelShader.Bind();

	for (auto& chunk : chunks)
	{
		chunk.second->Draw(camera);
	}
}