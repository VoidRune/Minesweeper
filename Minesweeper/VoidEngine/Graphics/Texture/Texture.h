#pragma once
#include <string>
#include "Graphics/GraphicsDevice.h"

class Texture
{
public:
	Texture() {};
	Texture(std::string name, bool linearFilter = false, bool flipTex = true);
	~Texture();
	void Bind(unsigned int slot);
	/* Supports 1 or 4 channels */
	void LoadFromData(void* data, int w, int h, int channels, bool linearFilter = false);
	int get_width() { return m_Width; };
	int get_height() { return m_Height; };
private:
	int m_Width;
	int m_Height;

	ID3D11Texture2D* pTexture;
	ID3D11SamplerState* pSampler;
	ID3D11ShaderResourceView* pTextureView;

};

class TextureArray
{
public:
	TextureArray(int arraySize);
	~TextureArray();
	//Adds a texture as next index in array
	//dimensions should be the same
	void AddTexture(std::string name, bool flipTex = true);
	//dimensions depend on last texture
	void Finish(bool linearFilter = false);
	void Bind(unsigned int slot);
	int get_width() { return m_Width; };
	int get_height() { return m_Height; };
private:
	int m_Width = 16;
	int m_Height = 16;

	unsigned char** data;
	D3D11_SUBRESOURCE_DATA* sd;
	int arraySize;
	int textureIndex = 0;
	ID3D11Texture2D* pTexture;
	ID3D11SamplerState* pSampler;
	ID3D11ShaderResourceView* pTextureView;
};

class TextureBatch
{
public:
	TextureBatch() {};
	/* Data is already freed in batchedTex.LoadFromData*/
	~TextureBatch() {};
	void AddTexture(std::string path);
	void Finish(bool linearFilter = false, bool flipTex = false);
	void Bind(unsigned int slot);
	vec4f get_coords(std::string key);
	int get_width() { return m_Width; };
	int get_height() { return m_Height; };
private:
	int m_Width = 16;
	int m_Height = 16;
	std::unordered_map<std::string, vec4f> coordinates;
	Pixel* data;

	int incX = 0;
	int incY = 0;
	int maxIncY = 0;
	int lineX = 0;
	int lineY = 0;

	Texture batchedTex;
};
