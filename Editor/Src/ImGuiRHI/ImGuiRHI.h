#pragma once
#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include <imgui/imgui.h>

class ImGuiRHI
{
public:
	static bool Initialize(Lemon::Renderer* renderer, const float width, const float height);

	static void NewFrame();

	static void Shutdown();

	static void RenderDrawData(ImDrawData* draw_data);

	static void OnResize(const float width, const float height);

};