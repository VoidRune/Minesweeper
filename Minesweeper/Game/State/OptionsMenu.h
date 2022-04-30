#pragma once
#include "Graphics/UI/Widgets.h"
#include <vector>

class OptionsMenu
{
public:
	OptionsMenu();
	~OptionsMenu();

	void Update();

private:

	//std::vector<Button> buttons;
	//Sprite sprite;
	bool isActive;
};