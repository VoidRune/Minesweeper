#include "Chunk.h"
#include "Utility/SimplexNoise.h"

Chunk::Chunk(vec2i chunkOffset, std::vector<Vertex>* vector) :
	chunkOffset(chunkOffset),
	meshDataReference(vector)
{
	blockData = new Block[CHUNK_MASS];
	neighbourBlockData = new uint8_t[CHUNK_MASS];
	segments.reserve(CHUNK_SEGMENTS);
	for (size_t i = 0; i < CHUNK_SEGMENTS; i++)
	{
		segments.emplace_back(i, chunkOffset);
	}
}

Chunk::~Chunk()
{
	if (blockData != nullptr) { delete blockData; }
	if (neighbourBlockData != nullptr) { delete neighbourBlockData; }
}

void Chunk::SetBlock(Block block, int x, int y, int z)
{
	blockData[to1DArray(x, y, z)] = block;
	setNeighborBlock(x, y, z);
	UpdateSegment(y / SEGMENT_HEIGHT);
}

void Chunk::setNeighborBlock(int x, int y, int z)
{
	uint8_t* block = &neighbourBlockData[to1DArray(x, y, z)];
	*block = 0;

	if (blockData[to1DArray(x, y, z)].id == 0) { return; }
	if (GetBlock(x, y, z - 1).id == 0) { *block |= 1 << 0; } /* Front	*/
	if (GetBlock(x, y, z + 1).id == 0) { *block |= 1 << 1; } /* Back	*/
	if (GetBlock(x + 1, y, z).id == 0) { *block |= 1 << 2; } /* Right	*/
	if (GetBlock(x - 1, y, z).id == 0) { *block |= 1 << 3; } /* Left	*/
	if (GetBlock(x, y + 1, z).id == 0) { *block |= 1 << 4; } /* Top	*/
	if (GetBlock(x, y - 1, z).id == 0) { *block |= 1 << 5; } /* Bottom */
	/* Ambient */
	/*
	if (*block == 0) { return; }
	if (GetBlock(x - 1, y - 1, z - 1) != 0) { *block |= 1 << 6; }
	//if (GetBlock(x, y - 1, z - 1) != 0) { *block |= 1 << 6; *block |= 1 << 9; }
	if (GetBlock(x + 1, y - 1, z - 1) != 0) { *block |= 1 << 9; }

	//if (GetBlock(x - 1, y, z - 1) != 0) { *block |= 1 << 6; *block |= 1 << 7; }
	//if (GetBlock(x + 1, y, z - 1) != 0) { *block |= 1 << 8; *block |= 1 << 9; }

	if (GetBlock(x, y + 1, z) == 0)
	{
		if (GetBlock(x, y + 1, z - 1) != 0) { *block |= 1 << 7; *block |= 1 << 8; }
		if (GetBlock(x - 1, y + 1, z) != 0) { *block |= 1 << 7; *block |= 1 << 11; }
		if (GetBlock(x + 1, y + 1, z) != 0) { *block |= 1 << 8; *block |= 1 << 12; }
		if (GetBlock(x, y + 1, z + 1) != 0) { *block |= 1 << 11; *block |= 1 << 12; }
	}
	if (GetBlock(x - 1, y + 1, z - 1) != 0) { *block |= 1 << 7; }
	if (GetBlock(x + 1, y + 1, z - 1) != 0) { *block |= 1 << 8; }
	if (GetBlock(x - 1, y + 1, z + 1) != 0) { *block |= 1 << 11; }
	if (GetBlock(x + 1, y + 1, z + 1) != 0) { *block |= 1 << 12; }
	//if (GetBlock(x - 1, y - 1, z) != 0) { *block |= 1 << 6; *block |= 1 << 10; }

	//if (GetBlock(x + 1, y - 1, z) != 0) { *block |= 1 << 9; *block |= 1 << 13; }


	if (GetBlock(x - 1, y - 1, z + 1) != 0) { *block |= 1 << 10; }
	//if (GetBlock(x, y - 1, z + 1) != 0) { *block |= 1 << 10; *block |= 1 << 13; }
	if (GetBlock(x + 1, y - 1, z + 1) != 0) { *block |= 1 << 13; }

	//if (GetBlock(x - 1, y, z + 1) != 0) { *block |= 1 << 10; *block |= 1 << 11; }
	//if (GetBlock(x + 1, y, z + 1) != 0) { *block |= 1 << 12; *block |= 1 << 13; }
	*/
}

void Chunk::CreateMeshData(vec2i offset)
{
	for (size_t segment = 0; segment < CHUNK_SEGMENTS; segment++)
	{
		UpdateSegment(segment);
	}
	meshLoaded = true;
}

void Chunk::UpdateSegment(uint32_t segmentIndex)
{
	meshDataReference->clear();
	for (int y = 0; y < SEGMENT_HEIGHT; y++)
	{
		if (segmentIndex * SEGMENT_HEIGHT + y > highestBlock) { break; }
		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				uint8_t* block = &neighbourBlockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)];
				if(*block == 0){ continue; }
				float f6 = ((*block >> 6) & 1U) ? 0.7f : 1.0f;
				float f7 = ((*block >> 7) & 1U) ? 0.7f : 1.0f;
				float f8 = ((*block >> 8) & 1U) ? 0.7f : 1.0f;
				float f9 = ((*block >> 9) & 1U) ? 0.7f : 1.0f;

				float f10 = ((*block >> 10) & 1U) ? 0.7f : 1.0f;
				float f11 = ((*block >> 11) & 1U) ? 0.7f : 1.0f;
				float f12 = ((*block >> 12) & 1U) ? 0.7f : 1.0f;
				float f13 = ((*block >> 13) & 1U) ? 0.7f : 1.0f;

				//Front
				if ((*block >> 0) & 1U)
				{
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 0 +  segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 0, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f6);
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 1 +  segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 0, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f7);
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 1 +  segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 1, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f8);
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 0 +  segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 1, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f9);
				}
				//Back
				if ((*block >> 1) & 1U)
				{
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 0 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 0, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f13);
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 1 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 0, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f12);
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 1 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 1, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f11);
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 0 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 1, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f10);
				}
				//Right
				if ((*block >> 2) & 1U)
				{
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 0 + segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 0, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f9);
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 1 + segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 0, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f8);
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 1 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 1, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f12);
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 0 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 1, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f13);
				}
				//Left
				if ((*block >> 3) & 1U)
				{
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 0 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 0, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f10);
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 1 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 0, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f11);
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 1 + segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 1, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f7);
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 0 + segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 1, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f6);
				}
				//Top
				if ((*block >> 4) & 1U)
				{
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 1 + segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 0, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f7);
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 1 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 0, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f11);
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 1 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 1, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f12);
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 1 + segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 1, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f8);
				}
				//Bottom
				if ((*block >> 5) & 1U)
				{
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 0 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 0, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f10);
					meshDataReference->emplace_back(x + 0 + chunkOffset.x * CHUNK_WIDTH, y + 0 + segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 0, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f6);
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 0 + segmentIndex * SEGMENT_HEIGHT, z + 0 + chunkOffset.y * CHUNK_WIDTH, 1, 1, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f9);
					meshDataReference->emplace_back(x + 1 + chunkOffset.x * CHUNK_WIDTH, y + 0 + segmentIndex * SEGMENT_HEIGHT, z + 1 + chunkOffset.y * CHUNK_WIDTH, 1, 0, blockData[to1DArray(x, y + segmentIndex * SEGMENT_HEIGHT, z)].id, f13);
				}
			}
		}
	}
	SetSegmentData(segmentIndex, meshDataReference->data(), meshDataReference->size());
}

void Chunk::SetSegmentData(uint32_t segmentIndex, void* data, uint32_t size)
{
	segments[segmentIndex].dataSize = size;
	segments[segmentIndex].buffer.SetData(data, size, sizeof(Vertex));
	if (size != 0) { segments[segmentIndex].hasMeshData = true; }
}

void Chunk::CreateNeighbourData(vec2i offset, Chunk* front, Chunk* right, Chunk* back, Chunk* left)
{
	neighbourChunks.reserve(4);
	neighbourChunks.push_back(front);
	neighbourChunks.push_back(right);
	neighbourChunks.push_back(back);
	neighbourChunks.push_back(left);

	memset(&neighbourBlockData[0], 0, CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT * sizeof(uint8_t));
	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		if (y > highestBlock) { break; }
		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				setNeighborBlock(x, y, z);
			}
		}
	}
}

Block Chunk::GetBlock(int x, int y, int z)
{
	if (x < 0)
	{
		if (neighbourChunks[0] != nullptr)
			return neighbourChunks[0]->blockData[to1DArray(CHUNK_WIDTH - 1, y, z)];
		else
			return { 0 };
	}
	if (x >= CHUNK_WIDTH)
	{
		if (neighbourChunks[2] != nullptr)
			return neighbourChunks[2]->blockData[to1DArray(0, y, z)];
		else
			return { 0 };
	}
	if (z < 0)
	{
		if (neighbourChunks[3] != nullptr)
			return neighbourChunks[3]->blockData[to1DArray(x, y, CHUNK_WIDTH - 1)];
		else
			return { 0 };
	}
	if (z >= CHUNK_WIDTH)
	{
		if (neighbourChunks[1] != nullptr)
			return neighbourChunks[1]->blockData[to1DArray(x, y, 0)];
		else
			return { 0 };
	}
	if (y < 0 || y >= CHUNK_HEIGHT) { return { 0 }; }

	return blockData[to1DArray(x, y, z)];
}

int Chunk::to1DArray(int x, int y, int z)
{
	return x + z * CHUNK_WIDTH + y * CHUNK_SLICE;
}

void Chunk::Draw(Camera3D* camera)
{
	for (size_t i = 0; i < segments.size(); i++)
	{
		if (segments[i].hasMeshData)
		{
			if (camera->isInFrustrum(segments[i].boundingBox))
			{
				segments[i].buffer.Bind();
				GraphicsDevice::Get().context->DrawIndexed(segments[i].dataSize * 1.5f, 0, 0);
			}
		}
	}
}