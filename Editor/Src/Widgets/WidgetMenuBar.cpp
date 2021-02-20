#include "WidgetMenuBar.h"

WidgetMenuBar::WidgetMenuBar(Lemon::Engine* engine) : Widget(engine)
{
	m_WidgetProp.bIsWindow = false;

}


void WidgetMenuBar::Tick()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("World"))
		{
			if (ImGui::MenuItem("New"))
			{

			}

			ImGui::Separator();

			if (ImGui::MenuItem("Load"))
			{

			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save"))
			{

			}

			if (ImGui::MenuItem("Save As..."))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("ImGui Metrics", nullptr, &m_ImGuiMetrics);
			ImGui::MenuItem("ImGui Style", nullptr, &m_ImGuiStyle);
			ImGui::MenuItem("ImGui Demo", nullptr, &m_ShowDemoWindow);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About", nullptr, &m_ShowAboutWindow);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (m_ImGuiMetrics) { ImGui::ShowMetricsWindow(&m_ImGuiMetrics); }
	if (m_ImGuiStyle) { ImGui::Begin("Style Editor", nullptr, ImGuiWindowFlags_NoDocking); ImGui::ShowStyleEditor(); ImGui::End(); }
	if (m_ShowDemoWindow) { ImGui::ShowDemoWindow(&m_ShowDemoWindow); }

	ShowAboutWindow();
}

void WidgetMenuBar::ShowAboutWindow()
{
	if (!m_ShowAboutWindow)
		return;


	ImGui::SetNextWindowFocus();
	ImGui::Begin("About", &m_ShowAboutWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);
	ImGui::Text("Lemon : %s", EngineVersion);
	ImGui::Text("Author : %s", "Lemon&Rem");
	ImGui::Text("GitHub : %s", "https://github.com/ckloveit/Lemon-Engine");

	ImGui::End();
}