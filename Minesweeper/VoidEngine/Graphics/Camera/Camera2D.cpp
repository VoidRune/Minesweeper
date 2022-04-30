#include "Camera2D.h"

Camera2D::Camera2D()
{
    projection = XMMatrixOrthographicOffCenterLH(0, 1, 0, 1, -10, 10);
    XMMATRIX data = XMMatrixTranspose(projection);
    constBuffer.Set(&data, 16 * sizeof(float), 1, 0);
}

Camera2D::~Camera2D()
{

}