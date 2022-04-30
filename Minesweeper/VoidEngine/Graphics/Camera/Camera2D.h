#pragma once
#include "Graphics/Buffers.h"

class Camera2D
{
public:
	Camera2D();
	~Camera2D();

private:

	ConstantBuffer constBuffer;
	XMMATRIX projection;
};