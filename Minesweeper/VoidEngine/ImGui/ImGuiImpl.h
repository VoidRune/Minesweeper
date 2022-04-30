#pragma once
#include "imgui_master/imgui.h"
#include "imgui_master/imgui_impl_win32.h"
#include "imgui_master/imgui_impl_dx11.h"

void ImGuiInit();
void ImGuiStartFrame();
void ImGuiEndFrame();
void ImGuiDestroy();