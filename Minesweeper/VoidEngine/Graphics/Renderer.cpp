#include "Renderer.h"

Renderer* Renderer::s_Instance = nullptr;

Renderer::Renderer() :
	font("res/Fonts/segoepr.ttf")
{
	s_Instance = this;

	shader = new Shader("res/Shaders/renderer.hlsl");
	shader2D = new Shader("res/Shaders/renderer2D.hlsl");
	fontShader = new Shader("res/Shaders/font.hlsl");
	textured2DShader = new Shader("res/Shaders/textured.hlsl");

	lineData.reserve(2 * maxLines);
	solidData.reserve(4 * maxSolidSquares);
	solid2DData.reserve(4 * maxSolidSquares);
	textured2DData.reserve(4 * maxSolidSquares);

	lineBuffer.SetData(nullptr, 2 * maxLines, sizeof(Vertex));
	solidBuffer.SetData(nullptr, 4 * maxSolidSquares, sizeof(Vertex));
	solid2DBuffer.SetData(nullptr, 4 * maxSolidSquares, sizeof(Vertex));
	textBuffer.SetData(nullptr, 4 * maxSolidSquares, sizeof(FontVertex));
	textured2DBuffer.SetData(nullptr, 4 * maxSolidSquares, sizeof(TexVertex));
}

Renderer::~Renderer()
{
	delete fontShader;
	delete shader;
	delete shader2D;
}

vec3f Renderer::GetNextRGB()
{
	m_currentHSV += 42.0f;
	m_currentHSV = (int)m_currentHSV % 360;
	return HSVtoRGB(m_currentHSV, 100.0f, 100.0f);
}

void Renderer::AddLine3D(float x1, float y1, float z1, float x2, float y2, float z2)
{
	lineData.emplace_back(x1, y1, z1, 1, 1, 1, 1);
	lineData.emplace_back(x2, y2, z2, 1, 1, 1, 1);
}

void Renderer::AddCube(float centerX, float centerY, float centerZ, float sideLength, vec3f color)
{
	float r, g, b;
	r = color.x;
	g = color.y;
	b = color.z;

	//Front
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	//Back																		
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
	//Right																	
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
	//Left																	
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	//Top																		
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY + sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	//Bottom																	 
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX - sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ - sideLength / 2.0f, r, g, b, 1.0f);
	solidData.emplace_back(centerX + sideLength / 2.0f, centerY - sideLength / 2.0f, centerZ + sideLength / 2.0f, r, g, b, 1.0f);
}

void Renderer::Add2DQuad(float centerX, float centerY, float z, float width, float height, vec3f color)
{
	solid2DData.emplace_back(centerX - width / 2.0f, centerY - height / 2.0f, z, color.x, color.y, color.z, 1.0f);
	solid2DData.emplace_back(centerX - width / 2.0f, centerY + height / 2.0f, z, color.x, color.y, color.z, 1.0f);
	solid2DData.emplace_back(centerX + width / 2.0f, centerY + height / 2.0f, z, color.x, color.y, color.z, 1.0f);
	solid2DData.emplace_back(centerX + width / 2.0f, centerY - height / 2.0f, z, color.x, color.y, color.z, 1.0f);
}

void Renderer::Add2DQuad(float centerX, float centerY, float z, float width, float height, vec4f color)
{
	solid2DData.emplace_back(centerX - width / 2.0f, centerY - height / 2.0f, z, color.x, color.y, color.z, color.w);
	solid2DData.emplace_back(centerX - width / 2.0f, centerY + height / 2.0f, z, color.x, color.y, color.z, color.w);
	solid2DData.emplace_back(centerX + width / 2.0f, centerY + height / 2.0f, z, color.x, color.y, color.z, color.w);
	solid2DData.emplace_back(centerX + width / 2.0f, centerY - height / 2.0f, z, color.x, color.y, color.z, color.w);
}

void Renderer::AddTexturedQuad(float centerX, float centerY, float z, float width, float height, vec4f uv)
{
	textured2DData.emplace_back(vec3f{centerX - width / 2.0f, centerY - height / 2.0f, z}, vec2f{uv.x, uv.w});
	textured2DData.emplace_back(vec3f{centerX - width / 2.0f, centerY + height / 2.0f, z}, vec2f{uv.x, uv.y});
	textured2DData.emplace_back(vec3f{centerX + width / 2.0f, centerY + height / 2.0f, z}, vec2f{uv.z, uv.y});
	textured2DData.emplace_back(vec3f{centerX + width / 2.0f, centerY - height / 2.0f, z}, vec2f{uv.z, uv.w});
}

void Renderer::AddText(std::string text, float x, float y, float z, float r, float g, float b)
{
	font.FillBufferWithTextData(text, TextData, x, y, z, r, g, b);
}

void Renderer::AddText(std::string text, vec3f position, vec3f color)
{
	font.FillBufferWithTextData(text, TextData, position.x, position.y, position.z, color.x, color.y, color.z);
}

void Renderer::Draw()
{
	m_currentHSV = 0;

	ResourceManager::Get().BindQuadIndexBuffer();

	if (textured2DData.size() != 0)
	{
		ResourceManager::Get().batchedTextures.at("uiTex").Bind(0);
		textured2DShader->Bind();
		textured2DBuffer.SetData(textured2DData.data(), textured2DData.size(), sizeof(TexVertex));
		textured2DBuffer.Bind();
		GraphicsDevice::Get().context->DrawIndexed(textured2DData.size() * 1.5f, 0, 0);

		textured2DData.clear();
	}

	shader->Bind();
	if (lineData.size() != 0)
	{
		GraphicsDevice::Get().context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		lineBuffer.SetData(lineData.data(), lineData.size(), sizeof(Vertex));
		lineBuffer.Bind();
		GraphicsDevice::Get().context->Draw(lineData.size(), 0);
		GraphicsDevice::Get().context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		lineData.clear();
	}
	if (solidData.size() != 0)
	{
		solidBuffer.SetData(solidData.data(), solidData.size(), sizeof(Vertex));
		solidBuffer.Bind();
		GraphicsDevice::Get().context->DrawIndexed(solidData.size() * 1.5f, 0, 0);

		solidData.clear();
	}
	if (solid2DData.size() != 0)
	{
		shader2D->Bind();
		solid2DBuffer.SetData(solid2DData.data(), solid2DData.size(), sizeof(Vertex));
		solid2DBuffer.Bind();
		GraphicsDevice::Get().context->DrawIndexed(solid2DData.size() * 1.5f, 0, 0);

		solid2DData.clear();
	}

	if (TextData.size() != 0)
	{
		textBuffer.SetData(TextData.data(), TextData.size(), sizeof(FontVertex));
		fontShader->Bind();
		font.BindFontTexture();
		textBuffer.Bind();
		GraphicsDevice::Get().context->DrawIndexed(TextData.size() * 1.5f, 0, 0);

		TextData.clear();
	}
}