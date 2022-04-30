#pragma once
#include "Graphics/Buffers.h"
#include "Graphics/Texture/Texture.h"
#include "Utility/Helper.h"
#include "Audio/AudioEngine.h"

struct Vertex2D
{
	float x, y, z;
	float u, v;
	Vertex2D(float x, float y, float z, float u, float v) :
		x(x), y(y), z(z), u(u), v(v) {}
};

struct Vertex2DArray
{
	float x, y, z;
	float u, v, w;
	Vertex2DArray(float x, float y, float z, float u, float v, float w) :
		x(x), y(y), z(z), u(u), v(v), w(w) {}
};

class ResourceManager
{
public:

	static inline ResourceManager& Get() { return *s_Instance; }
	ResourceManager();
	~ResourceManager() {};
	void BindQuadIndexBuffer();

	void AddTextureBatch(std::string key, TextureBatch& tex);
	void AddSound(std::string key, Sound& sound);
	std::unordered_map<std::string, TextureBatch&> batchedTextures;
	std::unordered_map<std::string, Sound&> sounds;
private:

	IndexBuffer quadIndexBuffer;

	static ResourceManager* s_Instance;

};