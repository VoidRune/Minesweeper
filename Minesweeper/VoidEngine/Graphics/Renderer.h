#pragma once
#include <vector>
#include "Graphics/GraphicsDevice.h"
#include "Graphics/Shader.h"
#include "Graphics/Buffers.h"
#include "Utility/Helper.h"
#include "Graphics/UI/Font.h"
#include "ResourceManager.h"

class Renderer
{
public:
	static inline Renderer& Get() { return *s_Instance; }
	Renderer();
	~Renderer();
	//adds a line with assigned points to the next draw call
	void AddLine3D(float x1, float y1, float z1, float x2, float y2, float z2);
	void AddCube(float centerX, float centerY, float centerZ, float sideLength, vec3f color = { 1, 1, 1 });
	void Add2DQuad(float centerX, float centerY, float z, float width, float height, vec3f color = { 1, 1, 1 });
	void Add2DQuad(float centerX, float centerY, float z, float width, float height, vec4f color = { 1, 1, 1, 1 });
	void AddText(std::string text, float x, float y, float z, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	void AddText(std::string text, vec3f position, vec3f color = { 1.0f, 1.0f, 1.0f });
	void AddTexturedQuad(float centerX, float centerY, float z, float width, float height, vec4f uv);
	void Draw();
private:

	vec3f GetNextRGB();

	struct Vertex
	{
		float x, y, z;
		float r, g, b, a;
		Vertex(float x, float y, float z, float r, float g, float b, float a) :
			x(x), y(y), z(z), r(r), g(g), b(b), a(a) {}
	};
	struct TexVertex
	{
		vec3f pos;
		vec2f uv;
		TexVertex(vec3f pos, vec2f uv) :
			pos(pos), uv(uv) {}
	};

	Shader* shader;
	Shader* shader2D;
	Shader* fontShader;
	Shader* textured2DShader;

	VertexBuffer lineBuffer;
	VertexBuffer solidBuffer;
	VertexBuffer solid2DBuffer;
	VertexBuffer textured2DBuffer;

	Font font;
	VertexBuffer textBuffer;

	std::vector<Vertex> lineData;
	std::vector<Vertex> solidData;
	std::vector<Vertex> solid2DData;
	std::vector<FontVertex> TextData;
	std::vector<TexVertex> textured2DData;

	long long maxLines = 4000;
	long long maxSolidSquares = 4000;

	float m_currentHSV = 0.0f;

	static Renderer* s_Instance;
};

