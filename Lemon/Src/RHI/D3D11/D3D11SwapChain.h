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
		D3D11SwapChain(D3D11DynamicRHI* D3DRHI, void* windowHandle, const uint32_t width, const uint32_t height, const ERHIFormat format);
		virtual ~D3D11SwapChain();

		virtual void* GetRHISwapChain() override;
		virtual void* GetRHIRenderTargetView() override;

		virtual bool Present() override;
	private:
		ComPtr<IDXGISwapChain> m_SwapChain;
		ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

		D3D11DynamicRHI* m_D3DRHI;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_BackBufferCount;
		ERHIFormat m_PixelFormat;
	};
}
