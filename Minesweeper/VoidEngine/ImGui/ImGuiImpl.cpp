#include "ImGuiImpl.h"
#include "Window/Window.h"
#include "Graphics/GraphicsDevice.h"

void ImGuiInit()
{
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(Window::Get().GetHWnd());
    ImGui_ImplDX11_Init(GraphicsDevice::Get().device, GraphicsDevice::Get().context);
}

void ImGuiStartFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiEndFrame()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiDestroy()
{
    ImGui_ImplWin32_Shutdown();
}