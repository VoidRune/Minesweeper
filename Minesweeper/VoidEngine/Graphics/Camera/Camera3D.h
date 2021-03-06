#pragma once
#include "Graphics/Buffers.h"
#include <DirectXCollision.h>

class EntityPlayer;
class Camera3D
{
public:
	friend class EntityPlayer;
	Camera3D();
	~Camera3D();

	bool isInFrustrum(BoundingBox box);
private:
	void UpdateProjection();

	float r = 20.0f;
	float theta = 0.0f;
	float phi = 0.0f;
	float pitch = 0.0f;
	float yaw = 90.0f;
	float roll = 0.0f;
	float fov = 70.0f;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;

	ConstantBuffer constBuffer;
	XMMATRIX projection;
	XMMATRIX view;
	XMMATRIX skyboxMatrix;

	BoundingFrustum frustrum;

	XMVECTOR* position;
	XMVECTOR forward;
	XMVECTOR up;
	XMVECTOR right;
	XMVECTOR worldUp;
};