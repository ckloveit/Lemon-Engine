#include "WidgetViewport.h"
#include "Renderer/Renderer.h"
#include "Renderer/SceneRenderTargets.h"
#include "RHI/RHIResources.h"

using namespace Lemon;

WidgetViewport::WidgetViewport(Lemon::Engine* engine)
	:Widget(engine)
{
	m_WidgetProp.Title = WidgetGlobal::g_WidgetViewportTitle;
	m_WidgetProp.Size = glm::vec2(400, 250);
	m_WidgetProp.WindowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	m_WidgetProp.Padding = glm::vec2(0.0f, 0.0f);
	m_Renderer = m_Engine->GetSystem<Renderer>();

}

void WidgetViewport::Tick()
{
	if (!m_Renderer)
		return;

	// Draw our render result texture into ImGuiViewport
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	ImTextureID textureID = m_Renderer->GetSceneRenderTargets()->GetSceneColorTexture()->GetNativeShaderResourceView();
	// fuck!!!
	// ImGui::Image uv1 should (1,1).otherwise it will cause our render result failed....
	ImGui::Image(textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 0), ImVec2(1, 1));
}

