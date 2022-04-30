#include "Camera3D.h"

Camera3D::Camera3D() :
    forward(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
    up(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))
{
	projection = XMMatrixPerspectiveFovLH((XM_PI / 180) * fov, Window::Get().GetDimensions().x / Window::Get().GetDimensions().y, nearPlane, farPlane);

    XMVECTOR temp = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
    temp = XMVectorSetX(temp, cos(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(pitch)));
    temp = XMVectorSetZ(temp, sin(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(pitch)));
    temp = XMVectorSetY(temp, sin(XMConvertToRadians(pitch)));
    forward = XMVector4Normalize(temp);

    XMVECTOR position = XMVectorSet(0.0f, 16.0f, 0.0f, 0.0f);

    view = XMMatrixLookAtLH(position, DirectX::XMVectorAdd(position, forward), up);
    skyboxMatrix = XMMatrixLookAtLH(XMVectorSet(0,0,0,0), DirectX::XMVectorAdd(XMVectorSet(0, 0, 0, 0), forward), up);
    XMMATRIX data[2];
    data[0] = XMMatrixTranspose(view * projection);
    data[1] = XMMatrixTranspose(skyboxMatrix * projection);
    constBuffer.Set(&data, 32 * sizeof(float), 0, 0);

    frustrum.CreateFromMatrix(frustrum, projection);
}

Camera3D::~Camera3D()
{

}

bool Camera3D::isInFrustrum(BoundingBox box)
{
    return frustrum.Intersects(box);
}

void Camera3D::UpdateProjection()
{
    XMMATRIX data[2];
    data[0] = XMMatrixTranspose(view * projection);
    data[1] = XMMatrixTranspose(skyboxMatrix * projection);
    constBuffer.Load(&data, 32 * sizeof(float));
    //frustrum.CreateFromMatrix(frustrum, projection);
    XMFLOAT3 pos;
    XMFLOAT4 ori;
    XMStoreFloat3(&pos, *position);
    XMStoreFloat4(&ori, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(-pitch - 90) + XM_PIDIV2, XMConvertToRadians(-yaw) + XM_PIDIV2, 0));
    frustrum.Origin = pos;
    frustrum.Orientation = ori;
}