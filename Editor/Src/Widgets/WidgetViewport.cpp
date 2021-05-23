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
/*
		if(input->GetKeyDown(KeyCode::Q)) m_GizmoType = -1;
		if(input->GetKeyDown(KeyCode::W)) m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		if(input->GetKeyDown(KeyCode::E)) m_GizmoType = ImGuizmo::OPERATION::SCALE;
		if(input->GetKeyDown(KeyCode::R)) m_GizmoType = ImGuizmo::OPERATION::ROTATE;
*/		
	}
	//Draw ToolBar
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

	DrawAxisGizmo();
}

void WidgetViewport::DrawGizmoHandle()
{
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	float windowWidth = (float)ImGui::GetWindowWidth();
	float windowHeight = (float)ImGui::GetWindowHeight();

	/*InputSystem* input1 = m_Renderer->GetEngine()->GetSystem<InputSystem>();
	if (input1->GetKeyDown(KeyCode::Z))
	{
		LEMON_CORE_INFO("windowWidth = {0}, windowHeight = {1}, m_ViewportSize.X = {2}, m_ViewportSize.Y = {3}",
			windowWidth, windowHeight, m_ViewportSize.x, m_ViewportSize.y);
		LEMON_CORE_INFO("ImGui::GetWindowPos().x = {0}, ImGui::GetWindowPos().y = {1}",
			ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		LEMON_CORE_INFO("startPosX = {0}, startPosY = {1}",
			startPosX, startPosY);
	}*/

	float diff = (windowHeight - m_ViewportSize.y);
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + diff, m_ViewportSize.x, m_ViewportSize.y);

	//ImGuizmo::SetRect(0, 0, windowWidth, windowHeight);
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
        (ImGuizmo::OPERATION)m_GizmoType, (ImGuizmo::MODE)m_GizmoMode, glm::value_ptr(transform),
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

glm::vec2 ScreenUVToScreenPosition(glm::vec2 screenUV, glm::vec2 ViewportOffset, glm::vec2 ViewportSize)
{
	glm::vec2 screenPos = glm::vec2(screenUV.x * ViewportSize.x, screenUV.y * ViewportSize.y);
	screenPos += ViewportOffset;
	return screenPos;
}

glm::vec2 WorldToScreenUV(glm::vec3 worldPos, glm::mat4 ViewProjection)
{
	glm::vec4 trans = ViewProjection * glm::vec4(worldPos, 1.0f);
	float invZ = 1.0f / trans.w;
	glm::vec2 ndcPos = glm::vec2(trans.x, trans.y) * invZ;
	glm::vec2 screenUV = ndcPos * 0.5f + glm::vec2(0.5f, 0.5f);
	screenUV.y = 1.0f - screenUV.y;
	return screenUV;
}

glm::vec3 ScreenUVToWorld(glm::vec2 screenUV, float zNear, glm::mat4 viewProjection)
{
	glm::vec3 clipPos;
	clipPos.x = screenUV.x * 2.0f - 1.0f;
	clipPos.y = screenUV.y * -2.0f + 1.0f;
	clipPos.z = zNear;

	glm::mat4 viewProjectionInvert = glm::inverse(viewProjection);
	glm::vec4 worldPos = viewProjectionInvert * glm::vec4(clipPos, 1.0);
	worldPos *= 1.0f / worldPos.w;
	return glm::vec3(worldPos.x, worldPos.y, worldPos.z);
}

void WidgetViewport::DrawAxisGizmo()
{	
	float windowWidth = (float)ImGui::GetWindowWidth();
	float windowHeight = (float)ImGui::GetWindowHeight();
	float diff = (windowHeight - m_ViewportSize.y);

	// Editor camera
	float viewportOffsetX = ImGui::GetWindowPos().x;
	float viewportOffsetY = ImGui::GetWindowPos().y + diff;
	float viewportWidth = m_ViewportSize.x;
	float viewportHeight = m_ViewportSize.y;

	// colors
	static const ImU32 directionColor[3] = { 0xFF715ED8, 0xFF25AA25, 0xFFCC532C };

	CameraComponent& camera = m_Engine->GetSystem<World>()->GetMainCamera().GetComponent<CameraComponent>();

	glm::mat4 ViewMatrix = camera.GetViewMatrix();
	glm::mat4 ProjectionMatrix = camera.GetProjectionMatrix();
	glm::mat4 viewProjection = ProjectionMatrix * ViewMatrix;
	
	const glm::vec2 cameraOriginScreenUV = WorldToScreenUV(camera.GetPosition() + camera.GetForwardVector() * 100.0f, viewProjection);
	glm::vec3 originWorldPos = ScreenUVToWorld(cameraOriginScreenUV, camera.GetPerspectiveNearClip(), viewProjection);
	const glm::vec2 cameraOriginScreenPos = ScreenUVToScreenPosition(cameraOriginScreenUV, glm::vec2(viewportOffsetX, viewportOffsetY), glm::vec2(viewportWidth, viewportHeight));
	

	glm::vec2 axisOriginScreenUV = glm::vec2(40.0f / viewportWidth, 1.0f - 50.0f / viewportHeight);
	const glm::vec2 Origin = ScreenUVToScreenPosition(axisOriginScreenUV, glm::vec2(viewportOffsetX, viewportOffsetY), glm::vec2(viewportWidth, viewportHeight));
	ImVec2 axisOrigin = ImVec2(Origin.x, Origin.y);
	ImVec2 axisOffset = ImVec2(cameraOriginScreenPos.x - axisOrigin.x , cameraOriginScreenPos.y - axisOrigin.y);

	// X
	glm::vec3 xAxisEndWorldPos = originWorldPos + glm::vec3(1, 0, 0) * 0.01f;

	glm::vec2 xAxisEndScreenUV = WorldToScreenUV(xAxisEndWorldPos, viewProjection);
	const glm::vec2 xAxisEndScreenPos = ScreenUVToScreenPosition(xAxisEndScreenUV, glm::vec2(viewportOffsetX, viewportOffsetY), glm::vec2(viewportWidth, viewportHeight));
	const ImVec2 xAxisEnd = ImVec2(xAxisEndScreenPos.x - axisOffset.x, xAxisEndScreenPos.y - axisOffset.y);

	ImGui::GetWindowDrawList()->AddLine(axisOrigin, xAxisEnd, directionColor[0], 3.0f);

	char axisXName[] = "X";
	ImGui::GetWindowDrawList()->AddText(ImVec2(xAxisEnd.x + 6, xAxisEnd.y + 6), directionColor[0], axisXName);

	// Y
	glm::vec3 yAxisEndWorldPos = originWorldPos + glm::vec3(0, 1, 0) * 0.01f;

	glm::vec2 yAxisEndScreenUV = WorldToScreenUV(yAxisEndWorldPos, viewProjection);
	const glm::vec2 yAxisEndScreenPos = ScreenUVToScreenPosition(yAxisEndScreenUV, glm::vec2(viewportOffsetX, viewportOffsetY), glm::vec2(viewportWidth, viewportHeight));
	const ImVec2 yAxisEnd = ImVec2(yAxisEndScreenPos.x - axisOffset.x, yAxisEndScreenPos.y - axisOffset.y);

	ImGui::GetWindowDrawList()->AddLine(axisOrigin, yAxisEnd, directionColor[1], 3.0f);
	char axisYName[] = "Y";
	ImGui::GetWindowDrawList()->AddText(ImVec2(yAxisEnd.x + 6, yAxisEnd.y + 6), directionColor[1], axisYName);
	// Z
	glm::vec3 zAxisEndWorldPos = originWorldPos + glm::vec3(0, 0, 1) * 0.01f;

	glm::vec2 zAxisEndScreenUV = WorldToScreenUV(zAxisEndWorldPos, viewProjection);
	const glm::vec2 zAxisEndScreenPos = ScreenUVToScreenPosition(zAxisEndScreenUV, glm::vec2(viewportOffsetX, viewportOffsetY), glm::vec2(viewportWidth, viewportHeight));
	const ImVec2 zAxisEnd = ImVec2(zAxisEndScreenPos.x - axisOffset.x, zAxisEndScreenPos.y - axisOffset.y);

	ImGui::GetWindowDrawList()->AddLine(axisOrigin, zAxisEnd, directionColor[2], 3.0f);
	char axisZName[] = "Z";
	ImGui::GetWindowDrawList()->AddText(ImVec2(zAxisEnd.x + 6, zAxisEnd.y + 6), directionColor[2], axisZName);
}

void WidgetViewport::DrawToolBar()
{
	ImGui::Indent();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.0f));

	static bool selected = false;
	auto io = ImGui::GetIO();
	// Select the Font which has ICON
	auto IconFont = io.Fonts->Fonts[2];
	ImGui::PushFont(IconFont);
	{
		if (selected)
			ImGui::PushStyleColor(ImGuiCol_Button, ImGuiEx::GetSelectedColor());
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
		//m_Editor->GetImGuizmoOperation() == ImGuizmo::TRANSLATE;
	
		ImGui::SameLine();
		static char* label[2] = { ICON_FA_HOME, ICON_FA_GLOBE };
		if (ImGui::Button(label[m_GizmoMode], ImVec2(32, 32)))
		{
			if (m_GizmoMode == ImGuizmo::MODE::WORLD)
				m_GizmoMode = ImGuizmo::MODE::LOCAL;
			else 
				m_GizmoMode = ImGuizmo::MODE::WORLD;
			//m_Editor->SetImGuizmoOperation(ImGuizmo::TRANSLATE);
		}
		ImGuiEx::Tooltip("Space Mode");
	}

	{
		//m_Editor->GetImGuizmoOperation() == ImGuizmo::TRANSLATE;
		bool bPushStyleColor = false;
		if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE)
		{
			bPushStyleColor = true;
			ImGui::PushStyleColor(ImGuiCol_Button, ImGuiEx::GetSelectedColor());
		}
		ImGui::SameLine(); 
		if (ImGui::Button(ICON_FA_ARROWS_ALT, ImVec2(32, 32)))
		{
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			//m_Editor->SetImGuizmoOperation(ImGuizmo::TRANSLATE);
		}

		if (bPushStyleColor)
			ImGui::PopStyleColor();
		ImGuiEx::Tooltip("Translate");
	}

	{
		bool bPushStyleColor = false;
		if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
		{
			bPushStyleColor = true;
			ImGui::PushStyleColor(ImGuiCol_Button, ImGuiEx::GetSelectedColor());
		}

		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_UNDO, ImVec2(32, 32)))
		{
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			//m_Editor->SetImGuizmoOperation(ImGuizmo::ROTATE);
		}
		if (bPushStyleColor)
			ImGui::PopStyleColor();
		ImGuiEx::Tooltip("Rotate");
	}

	{
		bool bPushStyleColor = false;
		if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
		{
			bPushStyleColor = true;
			ImGui::PushStyleColor(ImGuiCol_Button, ImGuiEx::GetSelectedColor());
		}

		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_EXPAND, ImVec2(32, 32)))
		{
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			//m_Editor->SetImGuizmoOperation(ImGuizmo::SCALE);
		}
		if (bPushStyleColor)
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
