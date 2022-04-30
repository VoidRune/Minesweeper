#pragma once
#include "Window/Window.h"
#include "Graphics/Renderer.h"


namespace gui
{
	bool Button(std::string title, float x, float y, float width, float height, vec3f color);
	bool Button(std::string title, float x, float y, float width, float height, vec4f uv1, vec4f uv2);
	bool ButtonPixels(std::string title, float x, float y, int width, int height, vec3f color);
	bool ButtonPixels(std::string title, float x, float y, int width, int height, vec4f uv1, vec4f uv2);

	bool ButtonArray(int* buttonID, int startButtonID, int xButtons, int yButtons, float x, float y, float width, float height);
	bool ButtonArray(int* buttonID, int startButtonID, int enabledButtons, int xButtons, int yButtons, float x, float y, float width, float height);

	void Slider(float* value, float min, float max, float x, float y, float width, float height, vec3f color);
	void Slider(int* value, int min, int max, float x, float y, float width, float height, vec3f color);

	void Sprite(float x, float y, float z, float width, float height, vec4f uv);
	void SpritePixels(float x, float y, float z, int width, int height, vec4f uv);
	void Text(std::string text, float x, float y, float z, vec3f color = {1, 1, 1});
	void CheckBox(bool* value, float x, float y, float width, vec3f color);
	void CheckBoxPixels(bool* value, float x, float y, int width, vec3f color);
	void InputStringBox(std::string& s, bool* isEnabled, int maxCharacters, float x, float y, float width, float height, vec3f color);
	//void TextPixels(float x, float y, float z, int width, int height, vec4f uv);
}