#pragma once
#include "Graphics/Buffers.h"
#include "Graphics/Shader.h"
#include "Utility/Time.h"

class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	void Draw();

private:
	VertexBuffer vertexBuffer;
	Time timer;
	vec4f time;
	ConstantBuffer timeOfDay;
	Shader skyBoxShader;
};