#pragma once
#include <imgui/imgui.h>
#include <string>

namespace ImGuiEx
{
	void Tooltip(const std::string& text);
	void Tooltip(const char* text);
	ImVec4 GetSelectedColor();
}