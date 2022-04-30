#include "OptionsMenu.h"
#include "Window/Input.h"
#include "Core/Network/Networking.h"

OptionsMenu::OptionsMenu()
	//sprite(0.5, 0.5, 0.75)
{
	//buttons.emplace_back(0.5, 0.7, 0.2);
	//buttons.emplace_back(0.5, 0.3, 0.2);
	isActive = false;
}

OptionsMenu::~OptionsMenu()
{

}

void OptionsMenu::Update()
{

	if (Input::IsKeyPressed(27)) { isActive = !isActive; }
	if (!isActive) { return; }

	//for (int i = 0; i < buttons.size(); i++)
	//{
	//	if (buttons[i].isHovered())
	//	{
	//		buttons[i].addToRenderer(1.0f);
	//		if (Input::IsLeftMouseKeyPressed())
	//		{
	//			if (i == 0)
	//			{
	//				Networking::Get().StartServer(60000);
	//			}
	//			else
	//			{
	//				Window::Get().CloseWindow();
	//			}
	//		}
	//	}
	//	else
	//	{
	//		buttons[i].addToRenderer(0.5f);
	//	}
	//}
	//sprite.addToRenderer(0.2f);
	Renderer::Get().Draw();
}