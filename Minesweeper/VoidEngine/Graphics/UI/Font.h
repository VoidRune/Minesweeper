#pragma once
#include "stb_truetype.h"
#include "Utility/Structs.h"
#include "Graphics/Buffers.h"
#include "Graphics/Texture/Texture.h"
#include <memory>
#include <vector>

struct FontVertex
{
	vec3f pos;
	vec3f col;
	vec2f uv;
	FontVertex(vec3f pos, vec3f col, vec2f uv) :
		pos(pos), col(col), uv(uv) {}
};

class Font
{
public:

	Font(const char* path);
	~Font();
	void BindFontTexture();
	void FillBufferWithTextData(std::string text, std::vector<FontVertex>& buffer, float x, float y, float z, float r, float g, float b);

private:

	void initFont(const char* path);
	float getWidth(std::string);
	Texture tex;

	struct GlyphInfo
	{
		vec3f positions[4];
		vec2f uvs[4];
		float offsetX = 0;
		float offsetY = 0;
	};

	GlyphInfo makeGlyphInfo(uint32_t character, float offsetX, float offsetY, float offsetZ);

	struct FontDescription
	{
		const uint32_t size = 64;
		const uint32_t atlasWidth = size * 8;
		const uint32_t atlasHeight = size * 8;
		const uint32_t oversampleX = 2;
		const uint32_t oversampleY = 2;
		const uint32_t firstChar = ' ';
		const uint32_t charCount = '~' - ' ' + 1;
		std::unique_ptr<stbtt_packedchar[]> charInfo;
	} fontDesc;

};