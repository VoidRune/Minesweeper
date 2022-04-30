#include "ResourceManager.h"
#include <vector>

ResourceManager* ResourceManager::s_Instance = nullptr;

ResourceManager::ResourceManager()
{
	s_Instance = this;
	/* Max 8192 quads drawn pre draw call */
	int maxIndices = 6 * 8192;
	std::vector<uint32_t> indices;
	indices.reserve(maxIndices);
	uint32_t offset = 0;
	for (size_t i = 0; i < maxIndices; i += 6)
	{
		indices.emplace_back(0 + offset);
		indices.emplace_back(1 + offset);
		indices.emplace_back(2 + offset);

		indices.emplace_back(2 + offset);
		indices.emplace_back(3 + offset);
		indices.emplace_back(0 + offset);

		offset += 4;
	}

	quadIndexBuffer.SetData(indices.data(), indices.size());
}

void ResourceManager::BindQuadIndexBuffer()
{
	quadIndexBuffer.Bind();
}

void ResourceManager::AddTextureBatch(std::string key, TextureBatch& tex)
{
	batchedTextures.emplace(key, tex);
}

void ResourceManager::AddSound(std::string key, Sound& sound)
{
	sounds.emplace(key, sound);
}