#pragma once
#include "Core/Core.h"
#include "RHI/RHI.h"
#include "RHI/RHISwapChain.h"
#include "RHI/D3D11/D3D11RHI.h"
#include "RHI/D3D11/D3D11DynamicRHI.h"

namespace Lemon
{
	class LEMON_API D3D11SwapChain : public RHISwapChain
	{
	public:
		D3D11SwapChain(D3D11DynamicRHI* D3DRHI, void* windowHandle, const uint32_t width, const uint32_t height, const ERHIPixelFormat format);
		virtual ~D3D11SwapChain();

		virtual bool ReSize(uint32_t width, uint32_t height) override;

		virtual bool Present() override;

		//=========RHI Resource====================
		virtual void* GetRHISwapChain() override;
		virtual void* GetRHIRenderTargetView() override;
		virtual void* GetRHIDepthStencilView() override;
		//=========================================
	private:
		HRESULT ResizeDepthStencilBuffer(uint32_t width, uint32_t height);

	private:
		IDXGISwapChain* m_SwapChain;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11Texture2D* m_DepthStencilBuffer = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;

		D3D11DynamicRHI* m_D3DRHI;
		uint32_t m_BackBufferCount;
	};
}
