#include "SkyBox.h"

SkyBox::SkyBox():
	skyBoxShader("res/Shaders/skybox.hlsl")
{
	vec3f* data = new vec3f[36];

	data[0 ] = { -0.5f, -0.5f,  0.5f };
	data[1 ] = { -0.5f,  0.5f,  0.5f };
	data[2 ] = {  0.5f,  0.5f,  0.5f };
	data[3 ] = {  0.5f,  0.5f,  0.5f };
	data[4 ] = {  0.5f, -0.5f,  0.5f };
	data[5 ] = { -0.5f, -0.5f,  0.5f };

	data[6 ] = {  0.5f, -0.5f,  0.5f };
	data[7 ] = {  0.5f,  0.5f,  0.5f };
	data[8 ] = {  0.5f,  0.5f, -0.5f };
	data[9 ] = {  0.5f,  0.5f, -0.5f };
	data[10] = {  0.5f, -0.5f, -0.5f };
	data[11] = {  0.5f, -0.5f,  0.5f };

	data[12] = {  0.5f, -0.5f, -0.5f };
	data[13] = {  0.5f,  0.5f, -0.5f };
	data[14] = { -0.5f,  0.5f, -0.5f };
	data[15] = { -0.5f,  0.5f, -0.5f };
	data[16] = { -0.5f, -0.5f, -0.5f };
	data[17] = {  0.5f, -0.5f, -0.5f };

	data[18] = { -0.5f, -0.5f, -0.5f };
	data[19] = { -0.5f,  0.5f, -0.5f };
	data[20] = { -0.5f,  0.5f,  0.5f };
	data[21] = { -0.5f,  0.5f,  0.5f };
	data[22] = { -0.5f, -0.5f,  0.5f };
	data[23] = { -0.5f, -0.5f, -0.5f };

	data[24] = { -0.5f,  0.5f,  0.5f };
	data[25] = { -0.5f,  0.5f, -0.5f };
	data[26] = {  0.5f,  0.5f, -0.5f };
	data[27] = {  0.5f,  0.5f, -0.5f };
	data[28] = {  0.5f,  0.5f,  0.5f };
	data[29] = { -0.5f,  0.5f,  0.5f };

	data[30] = { -0.5f, -0.5f, -0.5f };
	data[31] = { -0.5f, -0.5f,  0.5f };
	data[32] = {  0.5f, -0.5f,  0.5f };
	data[33] = {  0.5f, -0.5f,  0.5f };
	data[34] = {  0.5f, -0.5f, -0.5f };
	data[35] = { -0.5f, -0.5f, -0.5f };

	vertexBuffer.SetData(data, 36, sizeof(vec3f));
	delete data;

	time.x = 0.0f;
	time.y = 0.0f;
	time.z = 0.0f;
	time.w = 0.0f;

	timeOfDay.Set(&time, sizeof(vec4f), 1, true);
}

SkyBox::~SkyBox()
{

}

void SkyBox::Draw()
{
	time.x = timer.elapsed_sec();
	timeOfDay.Load(&time, sizeof(vec4f));

	skyBoxShader.Bind();
	vertexBuffer.Bind();
	GraphicsDevice::Get().context->Draw(36, 0);
}