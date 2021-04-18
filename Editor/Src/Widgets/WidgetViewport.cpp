#include "WidgetViewport.h"
#include "../ImGuiRHI/ImGuiExtension.h"

#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Renderer/SceneRenderTargets.h"
#include "RHI/RHIResources.h"
#include "RenderCore/Viewport.h"
#include "World/World.h"
#include "World/Components/CameraComponent.h"
#include "Math/Math.h"

// URL:https://github.com/juliettef/IconFontCppHeaders
// Icon used for ImGui
#include "../ImGuiRHI/Icons/IconsFontAwesome4.h"

#include "WidgetSceneHierachy.h"
#include "World/Components/TransformComponent.h"

#include <ImGuizmo/ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

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
	if(ImGui::IsWindowHovered())
	{
		InputSystem* input = m_Renderer->GetEngine()->GetSystem<InputSystem>();
		if(input->GetKeyDown(KeyCode::Q)) m_GizmoType = -1;
		if(input->GetKeyDown(KeyCode::W)) m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		if(input->GetKeyDown(KeyCode::E)) m_GizmoType = ImGuizmo::OPERATION::SCALE;
		if(input->GetKeyDown(KeyCode::R)) m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		
	}
	
	bool gameView = false;
	if(!gameView)
	{
		DrawToolBar();
	}
	// Draw our render result texture into ImGuiViewport
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	//
	bool bMouseDown = ImGui::IsMouseDown(0);
	if(bMouseDown && ImGui::IsWindowHovered())
	{
		m_bWidgetFocus = true;
	}
	if(bMouseDown && m_bWidgetFocus && !ImGuizmo::IsUsing())
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

	//Process Gizmo Hanldle if interactive into viewport
	if(WidgetSceneHierachy::SelectEntity && m_GizmoType != -1)
	{
		DrawGizmoHandle();
	}
}

void WidgetViewport::DrawGizmoHandle()
{
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	float windowWidth = (float)ImGui::GetWindowWidth();
	float windowHeight = (float)ImGui::GetWindowHeight();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

	// Camera
	//auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
	//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
	//const glm::mat4& cameraProjection = camera.GetProjection();
	//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

	// Editor camera
	CameraComponent& camera = m_Engine->GetSystem<World>()->GetMainCamera().GetComponent<CameraComponent>();
	
	const glm::mat4& cameraProjection = camera.GetProjectionMatrix();// m_EditorCamera.GetProjection();
	glm::mat4 cameraView = camera.GetViewMatrix();//m_EditorCamera.GetViewMatrix();

	// Entity transform
	auto& tc = WidgetSceneHierachy::SelectEntity.GetComponent<TransformComponent>();
	glm::mat4 transform = tc.GetTransform();

	// Snapping
	InputSystem* input = m_Renderer->GetEngine()->GetSystem<InputSystem>();
	bool snap = input->GetKeyDown(KeyCode::Ctrl_Left);
	float snapValue = 0.5f; // Snap to 0.5m for translation/scale
	// Snap to 45 degrees for rotation
	if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
		snapValue = 45.0f;

	float snapValues[3] = { snapValue, snapValue, snapValue };

	ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
        (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
        nullptr, snap ? snapValues : nullptr);

	if (ImGuizmo::IsUsing())
	{
		glm::vec3 translation, rotation, scale;
		Math::DecomposeTransform(transform, translation, rotation, scale);

		glm::vec3 deltaRotation = glm::degrees(rotation) - tc.Rotation;
		tc.Position = translation;
		tc.Rotation += deltaRotation;
		tc.Scale = scale;
	}
}
void WidgetViewport::DrawToolBar()
{
	ImGui::Indent();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.0f));

	bool selected = false;
	auto io = ImGui::GetIO();
	// Select the Font which has ICON
	auto IconFont = io.Fonts->Fonts[2];
	ImGui::PushFont(IconFont);
	{
		if (selected)
			ImGui::PushStyleColor(ImGuiCol_Text, ImGuiEx::GetSelectedColor());
		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_ARROWS, ImVec2(32, 32)))
		{
			//
			m_GizmoType = -1;
		}
		//m_Editor->SetImGuizmoOperation(4);

		if (selected)
			ImGui::PopStyleColor();
		ImGuiEx::Tooltip("Select");
	}

	ImGui::SameLine();
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
	ImGui::SameLine();

	{
		selected = false;//m_Editor->GetImGuizmoOperation() == ImGuizmo::TRANSLATE;
		if (selected)
			ImGui::PushStyleColor(ImGuiCol_Text, ImGuiEx::GetSelectedColor());
		ImGui::SameLine(); 
		if (ImGui::Button(ICON_FA_ARROWS_ALT, ImVec2(32, 32)))
		{
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			//m_Editor->SetImGuizmoOperation(ImGuizmo::TRANSLATE);
		}

		if (selected)
			ImGui::PopStyleColor();
		ImGuiEx::Tooltip("Translate");
	}

	{
		selected = false;// m_Editor->GetImGuizmoOperation() == ImGuizmo::ROTATE;
		if (selected)
			ImGui::PushStyleColor(ImGuiCol_Text, ImGuiEx::GetSelectedColor());

		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_UNDO, ImVec2(32, 32)))
		{
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			//m_Editor->SetImGuizmoOperation(ImGuizmo::ROTATE);
		}
		if (selected)
			ImGui::PopStyleColor();
		ImGuiEx::Tooltip("Rotate");
	}

	{
		selected = false;//m_Editor->GetImGuizmoOperation() == ImGuizmo::SCALE;
		if (selected)
			ImGui::PushStyleColor(ImGuiCol_Text, ImGuiEx::GetSelectedColor());

		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_EXPAND, ImVec2(32, 32)))
		{
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			//m_Editor->SetImGuizmoOperation(ImGuizmo::SCALE);
		}
		if (selected)
			ImGui::PopStyleColor();
		ImGuiEx::Tooltip("Scale");
	}

	//ImGui::SameLine();
	//ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
	//ImGui::SameLine();

	ImGui::PopFont();
	//
	ImGui::PopStyleColor();
	ImGui::Unindent();
}
