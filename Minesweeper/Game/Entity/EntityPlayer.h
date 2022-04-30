#pragma once
#include "Graphics/Camera/Camera3D.h"
#include "Core/World/ChunkManager.h"

class EntityPlayer
{
public:
	EntityPlayer();
	~EntityPlayer();

	void Update(float deltaTime, ChunkManager& chunkManager);
	void SetPosition(float x, float y, float z);
	void SetPosition(vec3f pos);
	XMVECTOR GetPositionPointer() { return position; };

	Camera3D* GetCamera() { return camera; }

	void Collide();
	vec3f GetPosition();
	XMVECTOR GetXMPosition();
private:
	Camera3D* camera;

	float sensitivity = 0.1f;
	float movementSpeed = 4.0f;

	vec3f lastPosition = {0, 0, 0};
	XMVECTOR position;
	XMVECTOR temp;
	//float eyeHeight = 0.85f;
	BoundingBox hitbox;

	vec2f windowCenter = { Window::Get().GetMonitorDimensions().x / 2, Window::Get().GetMonitorDimensions().y / 2 };
	POINT MousePos = {};

	bool lockCursor = true;
};