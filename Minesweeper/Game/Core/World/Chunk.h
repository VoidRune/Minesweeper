#pragma once
#include <cmath>
#include <vector>
#include "Graphics/Buffers.h"
#include "Graphics/Camera/Camera3D.h"
#include "Utility/Structs.h"

static const int32_t SEGMENT_WIDTH = 16;
static const int32_t SEGMENT_HEIGHT = 32;
static const int32_t CHUNK_SEGMENTS = 8;
static const int32_t CHUNK_WIDTH = SEGMENT_WIDTH;
static const int32_t CHUNK_HEIGHT = SEGMENT_HEIGHT * CHUNK_SEGMENTS;
static const int32_t CHUNK_SLICE = CHUNK_WIDTH * CHUNK_WIDTH;
static const int32_t CHUNK_MASS = CHUNK_SLICE * CHUNK_HEIGHT;

struct Segment
{
	VertexBuffer buffer;
	uint32_t dataSize = 0;

	BoundingBox boundingBox;

	bool hasMeshData = false;
	Segment(uint32_t segmentId, vec2i offset)
	{
		XMVECTOR p1;
		XMVECTOR p2;
		p1 = XMVectorSet(offset.x * CHUNK_WIDTH, segmentId * SEGMENT_HEIGHT, offset.y * CHUNK_WIDTH, 0.0f);
		p2 = XMVectorSet((offset.x + 1) * CHUNK_WIDTH, (segmentId + 1) * SEGMENT_HEIGHT, (offset.y + 1) * CHUNK_WIDTH, 0.0f);
		boundingBox.CreateFromPoints(boundingBox, p1, p2);
	};
	~Segment() {};
};

class ChunkManager;

struct Vertex
{
	float x, y, z;
	float u, v, c;
	float ambient;
	Vertex(float x, float y, float z, float u, float v, float c, float ambient) :
		x(x), y(y), z(z), u(u), v(v), c(c), ambient(ambient) {}
};

struct Block
{
	uint32_t id;
};

class Chunk
{
public:
	friend class ChunkManager;
	friend class TerrainGenerator;
	Chunk(vec2i chunkOffset, std::vector<Vertex>* vector);
	~Chunk();

	void Draw(Camera3D* camera);
	void SetBlock(Block block, int x, int y, int z);
	Block GetBlock(int x, int y, int z);
	void CreateNeighbourData(vec2i offset, Chunk* front, Chunk* right, Chunk* back, Chunk* left);
	void CreateMeshData(vec2i offset);
private:
	vec2i chunkOffset;
	Block* blockData = nullptr;
	uint8_t* neighbourBlockData = nullptr;
	uint32_t highestBlock = 0;

	bool hasBlockData = false;
	bool meshLoaded = false;

	std::vector<Segment> segments;
	std::vector<Chunk*> neighbourChunks;
	std::vector<Vertex>* meshDataReference;
	void UpdateSegment(uint32_t segmentIndex);
	void SetSegmentData(uint32_t segmentIndex, void* data, uint32_t size);
	int to1DArray(int x, int y, int z);
	void setNeighborBlock(int x, int y, int z);
};