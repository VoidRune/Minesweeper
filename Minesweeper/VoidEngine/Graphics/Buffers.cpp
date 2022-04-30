#include "Buffers.h"

/* VERTEX BUFFER */

VertexBuffer::VertexBuffer(const void* data, uint32_t size, uint32_t sizeOfVertex, bool dynamic)
{
    this->dynamic = dynamic;
    Set(data, size, sizeOfVertex);
}
void VertexBuffer::Set(const void* data, uint32_t size, uint32_t sizeOfVertex)
{
    vertex_stride = sizeOfVertex;
    vertex_offset = 0;

    if (size == 0)
    {
        LOG_CORE_WARN("Size of data for vertex buffer is 0");
        return;
    }

    D3D11_BUFFER_DESC vertex_buff_descr = {};
    vertex_buff_descr.ByteWidth = sizeOfVertex * size;
    vertex_buff_descr.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    vertex_buff_descr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertex_buff_descr.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;

    D3D11_SUBRESOURCE_DATA sr_data = { 0 };
    sr_data.pSysMem = data;

    HRESULT hr = GraphicsDevice::Get().device->CreateBuffer(
        &vertex_buff_descr,
        (data) ? &sr_data : nullptr,
        &vertex_buffer_ptr);
    assert(SUCCEEDED(hr));
}
VertexBuffer::~VertexBuffer()
{
	SafeRelease(&vertex_buffer_ptr);
}
void VertexBuffer::SetData(const void* data, uint32_t size, uint32_t sizeOfVertex)
{
    if (vertex_buffer_ptr == nullptr)
    {
        Set(data, size, sizeOfVertex);
        return;
    }

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    GraphicsDevice::Get().context->Map(vertex_buffer_ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, data, sizeOfVertex * size);
    GraphicsDevice::Get().context->Unmap(vertex_buffer_ptr, 0);
}
void VertexBuffer::Bind()
{
    if (!vertex_buffer_ptr)
    {
        LOG_CORE_WARN("vertex buffer is nullptr");
        return;
    }

    GraphicsDevice::Get().context->IASetVertexBuffers(
        0,
        1,
        &vertex_buffer_ptr,
        &vertex_stride,
        &vertex_offset);
}

/* INDEX BUFFER */
void IndexBuffer::SetData(const void* data, uint32_t size)
{
    if (index_buffer_ptr != nullptr)
    {
        LOG_CORE_WARN("index buffer already loaded");
        return;
    }
    else if (size == 0)
    {
        LOG_CORE_WARN("Size of data for index buffer is 0");
        return;
    }
    D3D11_BUFFER_DESC index_buff_descr = {};
    index_buff_descr.ByteWidth = sizeof(uint32_t) * size;
    index_buff_descr.Usage = D3D11_USAGE_DEFAULT;
    index_buff_descr.BindFlags = D3D11_BIND_INDEX_BUFFER;
    index_buff_descr.StructureByteStride = sizeof(uint32_t);
    index_buff_descr.CPUAccessFlags = 0;
    index_buff_descr.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sr_data = { 0 };
    sr_data.pSysMem = data;

    HRESULT hr = GraphicsDevice::Get().device->CreateBuffer(
        &index_buff_descr,
        &sr_data,
        &index_buffer_ptr);
    assert(SUCCEEDED(hr));
}
IndexBuffer::~IndexBuffer()
{
    if (index_buffer_ptr) { SafeRelease(&index_buffer_ptr); }
}
void IndexBuffer::Bind()
{
    if (!index_buffer_ptr)
    {
        LOG_CORE_WARN("index buffer is nullptr");
        return;
    }

    GraphicsDevice::Get().context->IASetIndexBuffer(
        index_buffer_ptr,
        DXGI_FORMAT_R32_UINT,
        0);
}

/* CONSTANT BUFFER */

ConstantBuffer::ConstantBuffer() {}

ConstantBuffer::ConstantBuffer(void* data, const int sizeOfData, int slot, bool bindPS)
{
    D3D11_BUFFER_DESC const_buff_descr = {};
    const_buff_descr.ByteWidth = sizeOfData;
    const_buff_descr.Usage = D3D11_USAGE_DYNAMIC;
    const_buff_descr.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    const_buff_descr.StructureByteStride = 0;
    const_buff_descr.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    const_buff_descr.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sr_data = { 0 };
    sr_data.pSysMem = data;
    sr_data.SysMemPitch = 0;
    sr_data.SysMemSlicePitch = 0;

    HRESULT hr = GraphicsDevice::Get().device->CreateBuffer(
        &const_buff_descr,
        &sr_data,
        &const_buffer_ptr);
    assert(SUCCEEDED(hr));

    if (bindPS)
    {
        GraphicsDevice::Get().context->PSSetConstantBuffers(slot, 1, &const_buffer_ptr);
    }
    else
    {
        GraphicsDevice::Get().context->VSSetConstantBuffers(slot, 1, &const_buffer_ptr);
    }
}
void ConstantBuffer::Set(void* data, const int sizeOfData, int slot, char bindShaderType)
{
    if (!const_buffer_ptr)
    {
        D3D11_BUFFER_DESC const_buff_descr = {};
        const_buff_descr.ByteWidth = sizeOfData;
        const_buff_descr.Usage = D3D11_USAGE_DYNAMIC;
        const_buff_descr.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        const_buff_descr.StructureByteStride = 0;
        const_buff_descr.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        const_buff_descr.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA sr_data = { 0 };
        sr_data.pSysMem = data;
        sr_data.SysMemPitch = 0;
        sr_data.SysMemSlicePitch = 0;

        HRESULT hr = GraphicsDevice::Get().device->CreateBuffer(
            &const_buff_descr,
            (data != nullptr) ? &sr_data : nullptr,
            &const_buffer_ptr);
        assert(SUCCEEDED(hr));

        if (bindShaderType == 0)
        {
            GraphicsDevice::Get().context->VSSetConstantBuffers(slot, 1, &const_buffer_ptr);
        }
        else if(bindShaderType == 1)
        {
            GraphicsDevice::Get().context->PSSetConstantBuffers(slot, 1, &const_buffer_ptr);
        }
        else if (bindShaderType == 2)
        {
            GraphicsDevice::Get().context->CSSetConstantBuffers(slot, 1, &const_buffer_ptr);
        }
    }
}
ConstantBuffer::~ConstantBuffer()
{
    SafeRelease(&const_buffer_ptr);
}
void ConstantBuffer::Load(void* data, const int sizeOfData)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    GraphicsDevice::Get().context->Map(const_buffer_ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, data, sizeOfData);
    GraphicsDevice::Get().context->Unmap(const_buffer_ptr, 0);
}
void ConstantBuffer::Bind(int slot)
{
    GraphicsDevice::Get().context->VSSetConstantBuffers(slot, 1, &const_buffer_ptr);
}