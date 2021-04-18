#include "WidgetViewport.h"

#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Renderer/SceneRenderTargets.h"
#include "RHI/RHIResources.h"
#include "RenderCore/Viewport.h"
#include "World/World.h"
#include "World/Components/CameraComponent.h"

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

void WidgetViewport::Tick(float deltaTime)
{
	if (!m_Renderer)
		return;

	// Draw our render result texture into ImGuiViewport
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	//
	bool bMouseDown = ImGui::IsMouseDown(0);
	if(bMouseDown && ImGui::IsWindowHovered())
	{
		m_bWidgetFocus = true;
	}
	if(bMouseDown && m_bWidgetFocus)
	{
		m_Renderer->GetEngine()->GetSystem<World>()->GetMainCamera().GetComponent<CameraComponent>().ProcessInputSystem(deltaTime);
	}

	if(!bMouseDown)
	{
		m_bWidgetFocus = false;
	}
	
	ImTextureID textureID = m_Renderer->GetSceneRenderTargets()->GetSceneColorTexture()->GetNativeShaderResourceView();
	// fuck!!!
	// ImGui::Image uv1 should (1,1).otherwise it will cause our render result failed....
	ImGui::Image(textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 0), ImVec2(1, 1));
}

