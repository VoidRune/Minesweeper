#pragma once
#include <vector>
#include "Graphics/GraphicsDevice.h"
#include "Graphics/Buffers.h"
#pragma comment(lib, "dxguid.lib")


class Shader
{
public:
	/* Loads vertex & pixels shader from same file */
	Shader(std::string path);
	/* Loads vertex & pixels shader from separate files */
	Shader(std::string vertexSPath, std::string pixelSPath);
	~Shader();

	void Bind();
private:
	void CreateVertexShader(std::string path);
	void CreatePixelShader(std::string path);
	HRESULT CreateInputLayout(ID3DBlob* pShaderBlob, ID3D11InputLayout** pInputLayout);

	ID3D11VertexShader* vertex_shader_ptr = {};
	ID3D11PixelShader* pixel_shader_ptr = {};

	ID3D11InputLayout* input_layout_ptr = {};
};

class ComputeShader
{
public:
	ComputeShader();
	~ComputeShader();

	/* inputSize -> size of input data in BYTES */
	/* in numthreads(X, Y, Z) X*Y*Z cant be greater than 1024 */
	void Create(std::string path, uint32_t numInputElements, uint32_t inputElementSize, uint32_t numOutputElements, uint32_t outputElementSize);
	void Dispatch(int thread_groupX, int thread_groupY, int thread_groupZ, void* inputData, void* outputData);

private:
	void LoadShader(std::string path);
	void LoadInput(uint32_t numInputElements, uint32_t inputElementSize);
	void LoadOutput(uint32_t numOutputElements, uint32_t outputElementSize);

	struct ConstantBuffer
	{
		float xPos;
		float yPos;
		float scale;
		float padding_;
	} cb;

	bool hasInputArray = false;
	uint32_t inputDataSize;
	float* data;

	ID3D11Buffer* const_buffer_ptr = nullptr;

	ID3D11ComputeShader* compute_shader_ptr;
	//Input
	ID3D11Buffer* mInputBuffer;
	ID3D11ShaderResourceView* mInputView;
	//Output
	ID3D11Buffer* mOutputBuffer;
	ID3D11Buffer* mOutputResultBuffer;
	ID3D11UnorderedAccessView* mOutputUAV;

};