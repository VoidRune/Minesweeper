#include "EntityPlayer.h"
#include "Core/Network/Networking.h"

EntityPlayer::EntityPlayer()
{
	camera = new Camera3D;

    SetCursorPos(windowCenter.x, windowCenter.y);

    camera->position = &position;
}

EntityPlayer::~EntityPlayer()
{
    delete camera;
}

void EntityPlayer::Update(float deltaTime, ChunkManager& chunkManager)
{
    if (Input::IsKeyPressed(27))
    {
        Window::Get().SetCursorVisibility(lockCursor);
        lockCursor = !lockCursor;
    }
    if (lockCursor)
    {
        GetCursorPos(&MousePos);
        camera->yaw += (windowCenter.x - MousePos.x) * sensitivity;
        camera->pitch += (windowCenter.y - MousePos.y) * sensitivity;
        SetCursorPos(windowCenter.x, windowCenter.y);
    }

    camera->yaw = fmod(camera->yaw, 360.0f);

    if (camera->pitch > 89.0f) { camera->pitch = 89.0f; }
    if (camera->pitch < -89.0f) { camera->pitch = -89.0f; }

    temp = XMVectorSetX(temp, cos(XMConvertToRadians(camera->yaw)) * cos(XMConvertToRadians(camera->pitch)));
    temp = XMVectorSetZ(temp, sin(XMConvertToRadians(camera->yaw)) * cos(XMConvertToRadians(camera->pitch)));
    temp = XMVectorSetY(temp, sin(XMConvertToRadians(camera->pitch)));
    camera->forward = XMVector3Normalize(temp);
    camera->right = XMVector3Normalize(XMVector3Cross(-camera->forward, camera->up));

    XMVECTOR front = XMVector3Normalize(XMVectorSet(XMVectorGetX(camera->forward), 0, XMVectorGetZ(camera->forward), 0));
    XMVECTOR direction = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    if (Input::IsKeyDown('W')) { direction += front; }
    if (Input::IsKeyDown('S')) { direction -= front; }
    if (Input::IsKeyDown('D')) { direction += camera->right; }
    if (Input::IsKeyDown('A')) { direction -= camera->right; }
    if (Input::IsKeyDown('E')) /* zoom */
    { camera->projection = XMMatrixPerspectiveFovLH((XM_PI / 180) * 15, Window::Get().GetDimensions().x / Window::Get().GetDimensions().y, camera->nearPlane, camera->farPlane); }
    else
    { camera->projection = XMMatrixPerspectiveFovLH((XM_PI / 180) * camera->fov, Window::Get().GetDimensions().x / Window::Get().GetDimensions().y, camera->nearPlane, camera->farPlane); }

    if (Input::IsKeyDown('F')) { movementSpeed = 64; }
    else { movementSpeed = 4; }

    //LOG_INFO("Player x: %f, y: %f, z: %f", XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position));
    //if(chunkManager.GetBlock(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)).id > 0)
    //{
    //    LOG_INFO("Collided");
    //}
    direction = XMVectorSetY(direction, 0);
    if (Input::IsKeyDown(' ')) { direction = XMVectorSetY(direction, 1); }
    if (Input::IsKeyDown(16)) { direction = XMVectorSetY(direction, -1); }

    direction = XMVector3Normalize(direction);
    position = XMVectorAdd(position, direction * deltaTime * movementSpeed);

    camera->view = XMMatrixLookAtLH(position, DirectX::XMVectorAdd(position, camera->forward), camera->up);
    camera->skyboxMatrix = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), DirectX::XMVectorAdd(XMVectorSet(0, 0, 0, 0), camera->forward), camera->up);

    camera->UpdateProjection();

    if (Networking::Get().IsConnected())
    {
        vec3f pos = GetPosition();
        if (pos != lastPosition)
        {
            message_header h;
            h.id = (int)GameMsg::Game_UpdatePlayer;
            h.socketId = Networking::Get().GetClientID();
            h.size = sizeof(pos);
            message msg = { h , &pos };
            Networking::Get().SendMessageToAll(msg);
            lastPosition = pos;
        }
    }
}

void EntityPlayer::SetPosition(float x, float y, float z)
{
    position = XMVectorSet(x, y, z, 0.0f);
}

void EntityPlayer::SetPosition(vec3f pos)
{
    position = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
}

vec3f EntityPlayer::GetPosition()
{
    return { XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position) };
}

XMVECTOR EntityPlayer::GetXMPosition()
{
    return { position };
}