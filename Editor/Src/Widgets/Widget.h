#pragma once
#include <string>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/glm.hpp>
//Lemon
#include "Core/Engine.h"

namespace WidgetGlobal
{
	const static std::string g_WidgetViewportTitle = "Viewport";
	const static std::string g_WidgetSceneHierachyTitle = "Scene";
	const static std::string g_WidgetPropertiesTitle = "Properties";

}


struct WidgetPropertiesConfig
{
	std::string Title;
	glm::vec2 Position = { -1.0f, -1.0f };
	glm::vec2 Size = { -1.0f, -1.0f };
	glm::vec2 MinSize = { -1.0f, -1.0f };
	glm::vec2 Padding = { -1.0f, -1.0f };

	uint32_t WindowFlags = ImGuiWindowFlags_NoCollapse;

	bool bIsWindow = true;
	bool bIsVisible = true;
	float Alpha = -1.0f;
};

class Widget
{
public:
	Widget(Lemon::Engine* engine)
	{
		m_Engine = engine;
		m_GUIWindow = nullptr;
	}

	bool Begin()
	{
		if (!m_WidgetProp.bIsWindow || !m_WidgetProp.bIsVisible)
			return false;

		//Reset ImGuiVarPushs
		m_ImGuiVarPushs = 0;

		//Position
		if (m_WidgetProp.Position.x != -1.0f && m_WidgetProp.Padding.y != -1.0f)
		{
			ImGui::SetNextWindowPos(ImVec2(m_WidgetProp.Position.x, m_WidgetProp.Position.y));
		}

		//Padding
		if (m_WidgetProp.Padding.x != -1.0f && m_WidgetProp.Padding.y != -1.0f)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(m_WidgetProp.Padding.x, m_WidgetProp.Padding.y));
			m_ImGuiVarPushs++;
		}

		//Alpha
		if (m_WidgetProp.Alpha != -1.0f)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_WidgetProp.Alpha);
			m_ImGuiVarPushs++;
		}
		// Size
		if (m_WidgetProp.Size.x != -1.0f && m_WidgetProp.Size.y != -1.0f)
		{
			ImGui::SetNextWindowSize(ImVec2(m_WidgetProp.Size.x, m_WidgetProp.Size.y), ImGuiCond_FirstUseEver);
		}

		//MinSize
		if (m_WidgetProp.MinSize.x != -1.0f && m_WidgetProp.MinSize.y != -1.0f)
		{
			ImGui::SetNextWindowSizeConstraints(ImVec2(m_WidgetProp.MinSize.x, m_WidgetProp.MinSize.y),
				ImVec2(FLT_MAX, FLT_MAX));
		}

		//Begin Window
		ImGui::Begin(m_WidgetProp.Title.c_str(), &m_WidgetProp.bIsVisible, m_WidgetProp.WindowFlags);
		m_bWindowHasBegin = true;

		return true;
	}

	virtual void Tick() = 0;

	bool End()
	{
		if (!m_bWindowHasBegin)
			return false;

		m_GUIWindow = ImGui::GetCurrentWindow();
		
		//End Window
		ImGui::End();
		ImGui::PopStyleVar(m_ImGuiVarPushs);
		m_bWindowHasBegin = false;

		return true;
	}

	const WidgetPropertiesConfig& GetWidgetConfig() const { return m_WidgetProp; }

protected:
	WidgetPropertiesConfig m_WidgetProp;

	ImGuiWindow* m_GUIWindow;
	Lemon::Engine* m_Engine;

private:
	uint8_t m_ImGuiVarPushs = 0;
	bool m_bWindowHasBegin = false;
};