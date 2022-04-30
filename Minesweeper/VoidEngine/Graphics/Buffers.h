#pragma once
#include "Graphics/GraphicsDevice.h"

class VertexBuffer
{
public:
	//Pass pointer to data, number of vertices, size of vertex, is buffer dynamic
	VertexBuffer(bool dynamic = true) { this->dynamic = dynamic; };
	VertexBuffer(const void* data, uint32_t size, uint32_t sizeOfVertex, bool dynamic = true);
	//May only be used once
	~VertexBuffer();
	void Bind();
	void SetData(const void* data, uint32_t size, uint32_t sizeOfVertex);
	ID3D11Buffer* get_buffer_ptr() { return  vertex_buffer_ptr; };
private:
	void Set(const void* data, uint32_t size, uint32_t sizeOfVertex);
	ID3D11Buffer* vertex_buffer_ptr = nullptr;
	UINT vertex_stride = 0;
	UINT vertex_offset = 0;
	bool dynamic;
};

class IndexBuffer
{
public:
	//Data must be of type unsigned int
	IndexBuffer() {};
	~IndexBuffer();
	void Bind();
	void SetData(const void* data, uint32_t size);
	ID3D11Buffer* get_buffer_ptr() { return  index_buffer_ptr; };
private:
	ID3D11Buffer* index_buffer_ptr = nullptr;
};

class ConstantBuffer
{
public:
	ConstantBuffer();
	ConstantBuffer(void* data, const int sizeOfData, int slot, bool bindPS);
	//may only be used for the first time
	//Data must be sized in multiples of 16
	//bindShaderType -> 0 = vertex, 1 = pixel, 2 = compute
	void Set(void* data, const int sizeOfData, int slot, char bindShaderType);
	~ConstantBuffer();
	void Load(void* data, const int sizeOfData);
	void Bind(int slot);
	ID3D11Buffer* get_buffer_ptr() { return  const_buffer_ptr; };
private:
	ID3D11Buffer* const_buffer_ptr = nullptr;
};