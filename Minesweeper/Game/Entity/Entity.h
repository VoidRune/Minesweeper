#pragma once
#include <unordered_map>
#include "Graphics/Buffers.h"
#include "Network/messages.h"

struct Vert
{
	float x, y, z;
	float u, v, c;
	float ambient;
	Vert(float x, float y, float z, float u, float v, float c, float ambient) :
		x(x), y(y), z(z), u(u), v(v), c(c), ambient(ambient) {}
};

class Entity
{
public:
	Entity() { m_players.reserve(8); };
	~Entity() {};

	void AddPlayer(uint32_t id, sPlayerDescription desc);
	void RemovePlayer(uint32_t id);
	void SetPlayerDescription(uint32_t id, sPlayerDescription desc);

	void Draw();
private:

	std::unordered_map<uint32_t, sPlayerDescription> m_players;
	int m_numOfPlayers = 2;
	std::vector<Vert> meshData;
	VertexBuffer m_vertexBuffer;
};