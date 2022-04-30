#include "Font.h"
#include "Utility/Helper.h"
#include "Window/Window.h"

Font::Font(const char* path)
{
	initFont(path);
};

Font::~Font()
{

};
void Font::BindFontTexture()
{
	tex.Bind(1);
}

void Font::initFont(const char* path)
{
	auto fontData = readFile(path);
	unsigned char* atlasData = new unsigned char[fontDesc.atlasWidth * fontDesc.atlasHeight * 4];
	//memset(atlasData, 0, font_.atlasWidth * font_.atlasHeight * sizeof(unsigned char));
	fontDesc.charInfo = std::make_unique<stbtt_packedchar[]>(fontDesc.charCount);
	
	stbtt_pack_context context;
	if (!stbtt_PackBegin(&context, atlasData, fontDesc.atlasWidth, fontDesc.atlasHeight, 0, 4, nullptr))
		//panic("Failed to initialize font");

		stbtt_PackSetOversampling(&context, fontDesc.oversampleX, fontDesc.oversampleY);
	if (!stbtt_PackFontRange(&context, fontData.data(), 0, fontDesc.size, fontDesc.firstChar, fontDesc.charCount, fontDesc.charInfo.get()))
		//panic("Failed to pack font");

		stbtt_PackEnd(&context);
	tex.LoadFromData(atlasData, fontDesc.atlasWidth, fontDesc.atlasHeight, 1, true);
}

void Font::FillBufferWithTextData(std::string text, std::vector<FontVertex>& buffer, float x, float y, float z, float r, float g, float b)
{
	float w = getWidth(text);
	float offsetX = x - (w / 2.0f), offsetY = -y + (fontDesc.size * 0.2f), offsetZ = z;
	
	for (auto c : text)
	{
		if (c == '\n')
		{
			offsetX = x;
			offsetY += fontDesc.size * 0.65f;
			continue;
		}

		const auto glyphInfo = makeGlyphInfo(c, offsetX, offsetY, offsetZ);
		offsetX = glyphInfo.offsetX;
		offsetY = glyphInfo.offsetY;

		buffer.emplace_back(FontVertex{ glyphInfo.positions[0], vec3f{r, g, b}, vec2f{glyphInfo.uvs[0].x, glyphInfo.uvs[0].y} });
		buffer.emplace_back(FontVertex{ glyphInfo.positions[1], vec3f{r, g, b}, vec2f{glyphInfo.uvs[1].x, glyphInfo.uvs[1].y} });
		buffer.emplace_back(FontVertex{ glyphInfo.positions[2], vec3f{r, g, b}, vec2f{glyphInfo.uvs[2].x, glyphInfo.uvs[2].y} });
		buffer.emplace_back(FontVertex{ glyphInfo.positions[3], vec3f{r, g, b}, vec2f{glyphInfo.uvs[3].x, glyphInfo.uvs[3].y} });
	}
}

Font::GlyphInfo Font::makeGlyphInfo(uint32_t character, float offsetX, float offsetY, float offsetZ)
{
	stbtt_aligned_quad quad;

	stbtt_GetPackedQuad(fontDesc.charInfo.get(), fontDesc.atlasWidth, fontDesc.atlasHeight,
		character - fontDesc.firstChar, &offsetX, &offsetY, &quad, 1);
	const auto xmin = (quad.x0 / Window::Get().GetDimensions().x);
	const auto xmax = (quad.x1 / Window::Get().GetDimensions().x);
	const auto ymin = (-quad.y1 / Window::Get().GetDimensions().y);
	const auto ymax = (-quad.y0 / Window::Get().GetDimensions().y);

	GlyphInfo info{};
	info.offsetX = offsetX;
	info.offsetY = offsetY;
	info.positions[0] = { xmin, ymin, offsetZ };
	info.positions[1] = { xmin, ymax, offsetZ };
	info.positions[2] = { xmax, ymax, offsetZ };
	info.positions[3] = { xmax, ymin, offsetZ };
	info.uvs[0] = { quad.s0, quad.t1 };
	info.uvs[1] = { quad.s0, quad.t0 };
	info.uvs[2] = { quad.s1, quad.t0 };
	info.uvs[3] = { quad.s1, quad.t1 };

	return info;
}

float Font::getWidth(std::string text) {
	float length = 0.0f;
	float height = 0.0f;
	stbtt_aligned_quad quad;
	for (auto c : text)
	{
		stbtt_GetPackedQuad(fontDesc.charInfo.get(), fontDesc.atlasWidth, fontDesc.atlasHeight,
			c - fontDesc.firstChar, &length, &height, &quad, 1);
	}
	return length;
}