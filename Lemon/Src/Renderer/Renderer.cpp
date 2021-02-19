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

		m_SwapChain = RHICreateSwapChain(windowData.Handle,
			static_cast<uint32_t>(m_Viewport.Width),
			static_cast<uint32_t>(m_Viewport.Height),
			RHI_Format_R8G8B8A8_Unorm);

		if (!m_SwapChain)
		{
			LEMON_CORE_ERROR("Failed to create swap chain");
			return false;
		}

		return true;
	}


	void Renderer::Tick(float deltaTime)
	{
		if (m_SwapChain)
		{
			RHIClearRenderTarget(m_SwapChain, { 1.0f, 1.0f, 0.0f, 1.0f });
			m_SwapChain->Present();
		}
	}

}