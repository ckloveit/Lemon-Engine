#include "LemonPCH.h"
#include "D3D11SwapChain.h"
#include "Log/Log.h"

namespace Lemon
{
	D3D11SwapChain::D3D11SwapChain(D3D11DynamicRHI* D3D11RHI, void* windowHandle, const uint32_t width, const uint32_t height, const ERHIPixelFormat format)
		:RHISwapChain(width,height,format),
		m_D3DRHI(D3D11RHI),
		m_BackBufferCount(1)
	{
		if (!D3D11RHI || !D3D11RHI->GetDevice())
		{
			LEMON_CORE_ERROR("Invalid device.");
			return;
		}
		// Validate window handle
		const auto hwnd = static_cast<HWND>(windowHandle);
		if (!hwnd || !IsWindow(hwnd))
		{
			LEMON_CORE_ERROR("Invalid hwnd.");
			return;
		}

		if (width < 0 || height < 0)
		{
			LEMON_CORE_ERROR("{0} x {1} is an invalid resolution", width, height);
			return;
		}

		// Get factory
		IDXGIFactory* dxgiFactory = nullptr;
		if (const auto & adapter = D3D11RHI->GetMainDeviceAdapter())
		{
			auto dxgi_adapter = static_cast<IDXGIAdapter*>(adapter->GetAdapter());
			if (dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgiFactory)) != S_OK)
			{
				LEMON_CORE_ERROR("Failed to get adapter's factory");
				return;
			}
		}
		else
		{
			LEMON_CORE_ERROR("Invalid primary adapter");
			return;
		}
		// Create swap chain
		{
			DXGI_SWAP_CHAIN_DESC desc = {};
			desc.BufferCount = static_cast<UINT>(m_BackBufferCount);
			desc.BufferDesc.Width = static_cast<UINT>(width);
			desc.BufferDesc.Height = static_cast<UINT>(height);
			desc.BufferDesc.Format = D3D11::D3D11Format[format];
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.OutputWindow = hwnd;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Windowed = TRUE;
			desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			desc.Flags = 0;

			if (!D3D11::DXGIErrorCheck(dxgiFactory->CreateSwapChain(D3D11RHI->GetDevice(), &desc, &m_SwapChain)))
			{
				LEMON_CORE_ERROR("Failed to create swapchain");
				return;
			}
		}

		// Create the render target
		ID3D11Texture2D* backbuffer = nullptr;
		auto result = m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("%s", D3D11::DXGIErrorToString(result));
			return;
		}
		result = D3D11RHI->GetDevice()->CreateRenderTargetView(backbuffer, nullptr, &m_RenderTargetView);
		backbuffer->Release();
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("%s", D3D11::DXGIErrorToString(result));
			return;
		}
	}

	D3D11SwapChain::~D3D11SwapChain()
	{
		D3D11::SafeRelease(m_SwapChain);
		D3D11::SafeRelease(m_RenderTargetView);

	}

	bool D3D11SwapChain::Present()
	{
		m_SwapChain->Present(0, 0);
		return true;
	}

	bool D3D11SwapChain::ReSize(uint32_t width, uint32_t height)
	{
		if (!m_SwapChain)
		{
			LEMON_CORE_ERROR("Invalid D3D11SwapChain Parameters.");
			return false;
		}
		// Store
		m_Width = width;
		m_Height = height;

		if (width < 0 || height < 0)
		{
			// Return true as when minimizing, a resolution
			// of 0,0 can be passed in, and this is fine.
			return true;
		}

		// Release previous stuff
		D3D11::SafeRelease(m_RenderTargetView);


		// Resize SwapChain buffers
		const UINT d3d11SwapChainFlags = 0;
		auto result = m_SwapChain->ResizeBuffers(m_BackBufferCount, static_cast<UINT>(width), static_cast<UINT>(height), D3D11::D3D11Format[m_PixelFormat], d3d11SwapChainFlags);
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("Failed to resize swapchain buffers, {0}.", D3D11::DXGIErrorToString(result));
			return false;
		}
		// Get SwapChain back-buffer
		ID3D11Texture2D* backbuffer = nullptr;
		result = m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("Failed to get swapchain buffer, {0}.", D3D11::DXGIErrorToString(result));
			return false;
		}

		// Create render target view
		result = m_D3DRHI->GetDevice()->CreateRenderTargetView(backbuffer, nullptr, &m_RenderTargetView);
		D3D11::SafeRelease(backbuffer);
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("Failed to create render target view, {0}.", D3D11::DXGIErrorToString(result));
			return false;
		}
		return true;
	}


	void* D3D11SwapChain::GetRHISwapChain()
	{
		return (void*)m_SwapChain;

	}
	void* D3D11SwapChain::GetRHIRenderTargetView()
	{
		return (void*)m_RenderTargetView;
	}
}