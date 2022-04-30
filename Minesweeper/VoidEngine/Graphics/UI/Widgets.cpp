#include "Widgets.h"

namespace gui
{

	bool Button(std::string title, float x, float y, float width, float height, vec3f color)
	{
		int winX = Window::Get().GetDimensions().x;
		int winY = Window::Get().GetDimensions().y;
		int minX = (x - width / 2.0f) * winX;
		int maxX = (x + width / 2.0f) * winX;
		int minY = (1.0f - y - height / 2.0f) * winY;
		int maxY = (1.0f - y + height / 2.0f) * winY;

		bool isHovered = false;
		if (Input::MousePosition().x > minX && Input::MousePosition().x < maxX)
		{
			if (Input::MousePosition().y > minY && Input::MousePosition().y < maxY)
			{
				Renderer::Get().Add2DQuad(x, y, -5.0f, width, height, color);
				isHovered = true;
				if (Input::IsLeftMouseKeyPressed())
				{
					Renderer::Get().AddText(title,
						{ x * Window::Get().GetDimensions().x, y * Window::Get().GetDimensions().y, -6 });
					return true;
				}
			}
		}
		Renderer::Get().AddText(title, 
			{ x * Window::Get().GetDimensions().x, y * Window::Get().GetDimensions().y, -6 });
		if(!isHovered)
			Renderer::Get().Add2DQuad(x, y, -5.0f, width, height, color + 0.2);
		return false;
	}
	bool Button(std::string title, float x, float y, float width, float height, vec4f uv1, vec4f uv2)
	{
		int winX = Window::Get().GetDimensions().x;
		int winY = Window::Get().GetDimensions().y;
		int minX = (x - width / 2.0f) * winX;
		int maxX = (x + width / 2.0f) * winX;
		int minY = (1.0f - y - height / 2.0f) * winY;
		int maxY = (1.0f - y + height / 2.0f) * winY;

		bool isHovered = false;
		if (Input::MousePosition().x > minX && Input::MousePosition().x < maxX)
		{
			if (Input::MousePosition().y > minY && Input::MousePosition().y < maxY)
			{
				Renderer::Get().AddTexturedQuad(x, y, -5.0f, width, height, uv2);
				isHovered = true;
				if (Input::IsLeftMouseKeyPressed())
				{
					Renderer::Get().AddText(title,
						{ x * Window::Get().GetDimensions().x, y * Window::Get().GetDimensions().y, -6 });

					return true;
				}
			}
		}
		Renderer::Get().AddText(title,
			{ x * Window::Get().GetDimensions().x, y * Window::Get().GetDimensions().y, -6 });
		if (!isHovered)
			Renderer::Get().AddTexturedQuad(x, y, -5.0f, width, height, uv1);
		return false;
	}

	bool ButtonPixels(std::string title, float x, float y, int width, int height, vec3f color)
	{
		int winX = Window::Get().GetDimensions().x;
		int winY = Window::Get().GetDimensions().y;
		float pw = width / (float)winX;
		float ph = height / (float)winY;
		int minX = (x - pw / 2.0f) * winX;
		int maxX = (x + pw / 2.0f) * winX;
		int minY = (1.0f - y - ph / 2.0f) * winY;
		int maxY = (1.0f - y + ph / 2.0f) * winY;

		bool isHovered = false;
		if (Input::MousePosition().x > minX && Input::MousePosition().x < maxX)
		{
			if (Input::MousePosition().y > minY && Input::MousePosition().y < maxY)
			{
				Renderer::Get().Add2DQuad(x, y, -5.0f, pw, ph, color);
				isHovered = true;
				if (Input::IsLeftMouseKeyPressed())
				{
					Renderer::Get().AddText(title,
						{ x * Window::Get().GetDimensions().x, y * Window::Get().GetDimensions().y, -6 });
					return true;
				}
			}
		}
		Renderer::Get().AddText(title,
			{ x * Window::Get().GetDimensions().x, y * Window::Get().GetDimensions().y, -6 });
		if (!isHovered)
			Renderer::Get().Add2DQuad(x, y, -5.0f, pw, ph, color + 0.2);
		return false;
	}

	bool ButtonPixels(std::string title, float x, float y, int width, int height, vec4f uv1, vec4f uv2)
	{
		int winX = Window::Get().GetDimensions().x;
		int winY = Window::Get().GetDimensions().y;
		float pw = width / (float)winX;
		float ph = height / (float)winY;
		int minX = (x - pw / 2.0f) * winX;
		int maxX = (x + pw / 2.0f) * winX;
		int minY = (1.0f - y - ph / 2.0f) * winY;
		int maxY = (1.0f - y + ph / 2.0f) * winY;

		bool isHovered = false;
		if (Input::MousePosition().x > minX && Input::MousePosition().x < maxX)
		{
			if (Input::MousePosition().y > minY && Input::MousePosition().y < maxY)
			{
				Renderer::Get().AddTexturedQuad(x, y, -5.0f, pw, ph, uv2);
				isHovered = true;
				if (Input::IsLeftMouseKeyPressed())
				{
					Renderer::Get().AddText(title,
						{ x * Window::Get().GetDimensions().x, y * Window::Get().GetDimensions().y, -6 });

					return true;
				}
			}
		}
		Renderer::Get().AddText(title,
			{ x * Window::Get().GetDimensions().x, y * Window::Get().GetDimensions().y, -6 });
		if (!isHovered)
			Renderer::Get().AddTexturedQuad(x, y, -5.0f, pw, ph, uv1);
		return false;
	}

	bool ButtonArray(int* buttonID, int startButtonID, int xButtons, int yButtons, float x, float y, float width, float height)
	{
		int winX = Window::Get().GetDimensions().x;
		int winY = Window::Get().GetDimensions().y;
		float xStart = x - (width / 2.0f) + width / (xButtons * 2);
		float yStart = y - (height / 2.0f) + height / (yButtons * 2);
		float bWidth = width / xButtons;
		float bHeight = height / yButtons;
		float mX = Input::MousePosition().x / winX;
		float mY = 1.0f - Input::MousePosition().y / winY;

		float xs = x - (width / 2.0f);
		float ys = y - (height / 2.0f);
		int bX = std::floor((mX - xs) / bWidth);
		int bY = std::floor((mY - ys) / bHeight);

		float rX = bWidth * 0.8f;
		float rY = bHeight * 0.8f;

		for(int j = 0; j < yButtons; j++)
		{
			for(int i = 0; i < xButtons; i++)
			{
				if (i == bX && j == bY)
				{
					Renderer::Get().Add2DQuad(xStart + i * width / xButtons, yStart + j * height / yButtons, -5.0f, rX, rY, { 0.1f, 0.1f, 0.1f });
				}
				else
				{
					Renderer::Get().Add2DQuad(xStart + i * width / xButtons, yStart + j * height / yButtons, -5.0f, rX, rY, { 0.3f, 0.3f, 0.3f });
				}
				Renderer::Get().AddText(numToString(startButtonID + i + (yButtons - j - 1) * xButtons),
					{ (xStart + i * width / xButtons) * winX, (yStart + j * height / yButtons) * winY, -6 });
			}
		}

		if (Input::IsLeftMouseKeyPressed() && bX >= 0 && bX < xButtons && bY >= 0 && bY < yButtons)
		{
			*buttonID = startButtonID + bX + (yButtons - bY - 1) * xButtons;
			return true;
		}
		return false;
	}
	bool ButtonArray(int* buttonID, int startButtonID, int enabledButtons, int xButtons, int yButtons, float x, float y, float width, float height)
	{
		int winX = Window::Get().GetDimensions().x;
		int winY = Window::Get().GetDimensions().y;
		float xStart = x - (width / 2.0f) + width / (xButtons * 2);
		float yStart = y - (height / 2.0f) + height / (yButtons * 2);
		float bWidth = width / xButtons;
		float bHeight = height / yButtons;
		float mX = Input::MousePosition().x / winX;
		float mY = 1.0f - Input::MousePosition().y / winY;

		float xs = x - (width / 2.0f);
		float ys = y - (height / 2.0f);
		int bX = std::floor((mX - xs) / bWidth);
		int bY = std::floor((mY - ys) / bHeight);

		float rX = bWidth * 0.8f;
		float rY = bHeight * 0.8f;

		for (int j = 0; j < yButtons; j++)
		{
			for (int i = 0; i < xButtons; i++)
			{
				float transparancy = (i + (yButtons - j - 1) * xButtons < enabledButtons) ? 1.0f : 0.4f;
				if (i == bX && j == bY && transparancy == 1.0f)
				{
					Renderer::Get().Add2DQuad(xStart + i * width / xButtons, yStart + j * height / yButtons, -5.0f, rX, rY, { 0.1f, 0.1f, 0.1f, transparancy });
				}
				else
				{
					Renderer::Get().Add2DQuad(xStart + i * width / xButtons, yStart + j * height / yButtons, -5.0f, rX, rY, { 0.3f, 0.3f, 0.3f, transparancy });
				}
				Renderer::Get().AddText(numToString(startButtonID + i + (yButtons - j - 1) * xButtons),
					{ (xStart + i * width / xButtons) * winX, (yStart + j * height / yButtons) * winY, -6 }, { transparancy , transparancy , transparancy });
			}
		}

		if (Input::IsLeftMouseKeyPressed() && bX >= 0 && bX < xButtons && bY >= 0 && bY < yButtons && (bX + (yButtons - bY - 1) * xButtons < enabledButtons))
		{
			*buttonID = startButtonID + bX + (yButtons - bY - 1) * xButtons;
			return true;
		}
		return false;
	}

	void Slider(float* value, float min, float max, float x, float y, float width, float height, vec3f color)
	{
		int winX = Window::Get().GetDimensions().x;
		int winY = Window::Get().GetDimensions().y;
		int minX = (x - width / 2.0f) * winX;
		int maxX = (x + width / 2.0f) * winX;
		int minY = (1.0f - y - (height * 3.0f) / 2.0f) * winY;
		int maxY = (1.0f - y + (height * 3.0f) / 2.0f) * winY;
		float range = (*value - min) / (max - min);

		if (Input::IsLeftMouseKeyDown())
		{
			if (Input::MousePosition().x > minX && Input::MousePosition().x < maxX)
			{
				if (Input::MousePosition().y > minY && Input::MousePosition().y < maxY)
				{
					range = (Input::MousePosition().x - minX) / (maxX - minX);
					*value = min + range * (max - min);
				}
			}
		}

		Renderer::Get().Add2DQuad(x - (width / 2.0f) + (range * width), y, -5.0f, 0.05f, height * 3.0f, color + 0.1f);
		Renderer::Get().Add2DQuad(x, y, -5.0f, width, height, color);
	}

	void Slider(int* value, int min, int max, float x, float y, float width, float height, vec3f color)
	{
		int winX = Window::Get().GetDimensions().x;
		int winY = Window::Get().GetDimensions().y;
		int minX = (x - width / 2.0f) * winX;
		int maxX = (x + width / 2.0f) * winX;
		int minY = (1.0f - y - (height * 3.0f) / 2.0f) * winY;
		int maxY = (1.0f - y + (height * 3.0f) / 2.0f) * winY;
		float range = ((float)*value - (float)min) / ((float)max - (float)min);

		if (Input::IsLeftMouseKeyDown())
		{
			if (Input::MousePosition().x > minX && Input::MousePosition().x < maxX)
			{
				if (Input::MousePosition().y > minY && Input::MousePosition().y < maxY)
				{
					range = (Input::MousePosition().x - minX) / (maxX - minX);
					*value = min + std::round(range * (max - min));
					range = ((float)*value - (float)min) / ((float)max - (float)min);
				}
			}
		}

		Renderer::Get().Add2DQuad(x - (width / 2.0f) + (range * width), y, -5.0f, 0.05f, height * 3.0f, color + 0.1f);
		Renderer::Get().Add2DQuad(x, y, -5.0f, width, height, color);

	}

	void Sprite(float x, float y, float z, float width, float height, vec4f uv)
	{
		Renderer::Get().AddTexturedQuad(x, y, z, width, height, uv);
	}

	void SpritePixels(float x, float y, float z, int width, int height, vec4f uv)
	{
		vec2f s = Window::Get().GetDimensions();
		Renderer::Get().AddTexturedQuad(x, y, z, width / s.x , height / s.y, uv);
	}

	void Text(std::string text, float x, float y, float z, vec3f color)
	{
		vec2f s = Window::Get().GetDimensions();
		Renderer::Get().AddText(text, { x * s.x, y * s.y, z }, color);
	}

	void CheckBox(bool* value, float x, float y, float width, vec3f color)
	{
		int winX = Window::Get().GetDimensions().x;
		int winY = Window::Get().GetDimensions().y;
		int minX = (x - width / 2.0f) * winX;
		int maxX = (x + width / 2.0f) * winX;
		int minY = (1.0f - y - width / 2.0f) * winY;
		int maxY = (1.0f - y + width / 2.0f) * winY;

		bool isHovered = false;
		if (Input::MousePosition().x > minX && Input::MousePosition().x < maxX)
		{
			if (Input::MousePosition().y > minY && Input::MousePosition().y < maxY)
			{
				if (Input::IsLeftMouseKeyPressed())
				{
					*value = !(*value);
				}
			}
		}
		Renderer::Get().Add2DQuad(x, y, -5.1f, width * 0.85f, width * 0.85f, color);
		Renderer::Get().Add2DQuad(x, y, -5.0f, width, width, color + 0.2);
		if (*value)
		{
			Renderer::Get().AddText("X",
				{ x * Window::Get().GetDimensions().x, y * Window::Get().GetDimensions().y, -6 });
		}
	}

	void CheckBoxPixels(bool* value, float x, float y, int width, vec3f color)
	{
		int winX = Window::Get().GetDimensions().x;
		int winY = Window::Get().GetDimensions().y;
		float pw = width / (float)winX;
		float ph = width / (float)winY;
		int minX = (x - pw / 2.0f) * winX;
		int maxX = (x + pw / 2.0f) * winX;
		int minY = (1.0f - y - ph / 2.0f) * winY;
		int maxY = (1.0f - y + ph / 2.0f) * winY;

		bool isHovered = false;
		if (Input::MousePosition().x > minX && Input::MousePosition().x < maxX)
		{
			if (Input::MousePosition().y > minY && Input::MousePosition().y < maxY)
			{
				if (Input::IsLeftMouseKeyPressed())
				{
					*value = !(*value);
				}
			}
		}
		Renderer::Get().Add2DQuad(x, y, -5.1f, pw * 0.85f, ph * 0.85f, color);
		Renderer::Get().Add2DQuad(x, y, -5.0f, pw, ph, color + 0.2);
		if (*value)
		{
			Renderer::Get().AddText("X",
				{ x * Window::Get().GetDimensions().x, y * Window::Get().GetDimensions().y, -6 });
		}
	}

	void InputStringBox(std::string& s, bool* isEnabled, int maxCharacters, float x, float y, float width, float height, vec3f color)
	{
		int winX = Window::Get().GetDimensions().x;
		int winY = Window::Get().GetDimensions().y;
		int minX = (x - width / 2.0f) * winX;
		int maxX = (x + width / 2.0f) * winX;
		int minY = (1.0f - y - height / 2.0f) * winY;
		int maxY = (1.0f - y + height / 2.0f) * winY;

		bool inside = false;
		if (Input::IsLeftMouseKeyPressed())
		{
			if (Input::MousePosition().x > minX && Input::MousePosition().x < maxX)
			{
				if (Input::MousePosition().y > minY && Input::MousePosition().y < maxY)
				{
					inside = true;
				}
			}
			*isEnabled = inside;
		}

		char c = 0;
		while (*isEnabled && Input::getCharInputBuffer(&c))
		{
			if (c == 8 && s.length() != 0)
				s.pop_back();
			else if(s.length() < maxCharacters && c > 32)
				s += c;
		}

		Renderer::Get().Add2DQuad(x, y, -5.1f, width - 0.015f, height - 0.015f, color);
		Renderer::Get().Add2DQuad(x, y, -5.0f, width, height, ((*isEnabled) ? vec3f{ 0.6f, 0.6f, 0.6f } : vec3f{ 0.1f, 0.1f, 0.1f }));
		Renderer::Get().AddText(s,
			{ x * Window::Get().GetDimensions().x, y * Window::Get().GetDimensions().y, -6 });
	}
}