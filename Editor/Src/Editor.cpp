#include "Editor.h"
#include <windows.h>
#include <imgui/imgui.h>

#include "Core/Engine.h"
#include "Renderer/Renderer.h"
#include "RHI/RHISwapChain.h"
#include "Core/Timer.h"

//==========Widgets==================
#include "Widgets/WidgetViewport.h"
#include "Widgets/WidgetSceneHierachy.h"
#include "Widgets/WidgetProperties.h"
#include "Widgets/WidgetMenuBar.h"

#include "ImGuiRHI/ImGuiRHI.h"
#include "Input/InputSystem.h"

#if LEMON_GRAPHICS_D3D11
#include "ImGuiRHI/Implementation/imgui_impl_dx11.h"
#include "ImGuiRHI/Implementation/imgui_impl_win32.h"
#endif

using namespace std;
using namespace Lemon;

namespace EditorGlobal
{
	Lemon::Renderer* g_Renderer = nullptr;

	const char* g_DockspaceName = "EditorDockspace";

	WidgetViewport* g_EditorViewport = nullptr;
	
}


Editor::~Editor()
{

}

void Editor::OnWindowMessage(WindowData& windowData)
{
	// During window creation, Windows fire off a couple of messages, m_initializing is to prevent that spamming.
	if (!m_Initialized)
	{
		// Create engine
		m_Engine = make_unique<Engine>(windowData);

		// Acquire the System we need
		EditorGlobal::g_Renderer = m_Engine->GetSystem<Lemon::Renderer>();

		InitImGui(windowData);

		m_Initialized = true;
	}
	else if (m_Initialized)
	{
		// Updated ImGui with message (if showing)
		if (true)
		{
#if LEMON_GRAPHICS_D3D11
			ImGui_ImplWin32_WndProcHandler(
				static_cast<HWND>(windowData.Handle),
				static_cast<uint32_t>(windowData.Message),
				static_cast<int64_t>(windowData.Wparam),
				static_cast<uint64_t>(windowData.Lparam)
			);
#endif
		}
		// Passing zero dimensions will cause the swapchain to not present at all
		uint32_t width = static_cast<uint32_t>(windowData.Width);
		uint32_t height = static_cast<uint32_t>(windowData.Height);

		// if size change
		if (EditorGlobal::g_Renderer->GetSwapChain()->GetWidth() != width || EditorGlobal::g_Renderer->GetSwapChain()->GetHeight() != height)
		{
			EditorGlobal::g_Renderer->GetSwapChain()->ReSize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
		}

		m_Engine->SetWindowData(windowData);
	}
}
void Editor::OnTick()
{
	// Check if OnResize
	Lemon::Viewport viewport = m_Engine->GetSystem<Renderer>()->GetViewport();
	glm::vec2 editorViewportSize = EditorGlobal::g_EditorViewport->GetViewportSize();
	
	if((viewport.Width > 0.0f && viewport.Height > 0.0f && editorViewportSize.x > 0.0f && editorViewportSize.y > 0.0f) &&  // zero sized framebuffer is invalid
	   (viewport.Width != EditorGlobal::g_EditorViewport->GetViewportSize().x ||
		viewport.Height != EditorGlobal::g_EditorViewport->GetViewportSize().y))
	{
		EditorGlobal::g_Renderer->OnResize(EditorGlobal::g_EditorViewport->GetViewportSize().x, EditorGlobal::g_EditorViewport->GetViewportSize().y);
	}
	
	// Engine - tick
	m_Engine->Tick();
	//Editor Tick
	{
		// ImGui - start frame
		ImGuiRHI::NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// Editor update
		WidgetsTick(m_Engine->GetSystem<Timer>()->GetDeltaTimeSec());

		// input system need special process
		m_Engine->GetSystem<InputSystem>()->SetFrameEnd();
		
		// ImGui implementation - end frame
		ImGui::Render();
		ImGuiRHI::RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
	
}

void Editor::InitImGui(const WindowData& windowData)
{
	// @Copy code from Dear ImGui Demo
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
	io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto/Roboto-Bold.ttf", 18.0f);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto/Roboto-Regular.ttf", 18.0f);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	SetDarkThemeColors();

	// ImGui backend setup
	ImGui_ImplWin32_Init(windowData.Handle);
	ImGuiRHI::Initialize(EditorGlobal::g_Renderer, static_cast<float>(windowData.Width), static_cast<float>(windowData.Height));

	// Create all ImGui widgets
	WidgetsCreate();
}

void Editor::WidgetsCreate()
{
	m_Widgets.emplace_back(make_unique<WidgetMenuBar>(m_Engine.get()));
	
	m_Widgets.emplace_back(make_unique<WidgetViewport>(m_Engine.get()));
	EditorGlobal::g_EditorViewport = static_cast<WidgetViewport*>(m_Widgets.back().get());
	
	m_Widgets.emplace_back(make_unique<WidgetSceneHierachy>(m_Engine.get()));
	m_Widgets.emplace_back(make_unique<WidgetProperties>(m_Engine.get()));
}

void Editor::WidgetsTick(float deltaTime)
{
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		BeginDockSpace();
	}

	for (auto& widget : m_Widgets)
	{
		widget->Begin();
		widget->Tick(deltaTime);
		widget->End();
	}

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		EndDockSpace();
	}
}

void Editor::BeginDockSpace()
{
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(EditorGlobal::g_DockspaceName, &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	// Dock space
	const auto dockspaceId = ImGui::GetID(EditorGlobal::g_DockspaceName);
	if (!ImGui::DockBuilderGetNode(dockspaceId))
	{
		// Reset current docking state
		ImGui::DockBuilderRemoveNode(dockspaceId);
		ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_None);
		ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);

		ImGuiID dockMainId = dockspaceId;
		ImGuiID dockLeftId;
		ImGuiID dockRightId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.2f, nullptr, &dockLeftId);
		ImGuiID dockRightUpId;
		ImGuiID dockRightDownId = ImGui::DockBuilderSplitNode(dockRightId, ImGuiDir_Down, 0.6f, nullptr, &dockRightUpId);

		ImGui::DockBuilderDockWindow(WidgetGlobal::g_WidgetViewportTitle.c_str(), dockLeftId);
		ImGui::DockBuilderDockWindow(WidgetGlobal::g_WidgetSceneHierachyTitle.c_str(), dockRightUpId);
		ImGui::DockBuilderDockWindow(WidgetGlobal::g_WidgetPropertiesTitle.c_str(), dockRightDownId);

	}
	ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
}

void Editor::EndDockSpace()
{
	ImGui::End();
}

void Editor::SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}