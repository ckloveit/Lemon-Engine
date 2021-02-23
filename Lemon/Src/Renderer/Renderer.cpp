#include "LemonPCH.h"
#include "Renderer.h"
#include "Core/Engine.h"
#include "RHI/RHI.h"
#include "RHI/DynamicRHI.h"
#include "Log/Log.h"
#include "RHI/RHISwapChain.h"

namespace Lemon
{
	Renderer::Renderer(Engine* engine)
		:ISystem(engine)
	{

	}
	Renderer::~Renderer()
	{
		RHIExit();
	}

	bool Renderer::Initialize()
	{
		// Resolution, viewport and swapchain default to whatever the window size is
		const WindowData& windowData = m_Engine->GetWindowData();

		// Set viewport
		m_Viewport.Width = windowData.Width;
		m_Viewport.Height = windowData.Height;

		// Init RHI
		RHIInit();

		m_RHISwapChain = RHICreateSwapChain(windowData.Handle,
			static_cast<uint32_t>(m_Viewport.Width),
			static_cast<uint32_t>(m_Viewport.Height),
			RHI_PF_R8G8B8A8_Unorm);

		if (!m_RHISwapChain)
		{
			LEMON_CORE_ERROR("Failed to create swap chain");
			return false;
		}

		// Create RHICommandList
		m_RHICommandList = RHICreateCommandList(this);
		// Create RenderTargetTextures
		m_SceneRenderTargets = CreateRef<SceneRenderTargets>(m_Viewport.Width, m_Viewport.Height);
		m_SceneRenderTargets->Allocate(m_RHICommandList);

		return true;
	}


	void Renderer::Tick(float deltaTime)
	{
		m_RHICommandList->RHIClearRenderTarget(GetSceneRenderTargets()->GetSceneColorTexture(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		/*
		if (m_RHISwapChain)
		{
			RHIClearRenderTarget(m_RHISwapChain, { 1.0f, 1.0f, 0.0f, 1.0f });
		}*/

	}

}