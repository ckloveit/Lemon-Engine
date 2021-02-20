#pragma once
#include "Core/Core.h"
#include "RHI.h"

namespace Lemon
{
	class LEMON_API RHISwapChain
	{
	public:
		RHISwapChain(uint32_t width, uint32_t height, ERHIFormat pixelFormat)
			:m_Width(width),m_Height(height),m_PixelFormat(pixelFormat)
		{}

		virtual ~RHISwapChain() = default;

		virtual bool Present() = 0;

		virtual bool ReSize(uint32_t width, uint32_t height) = 0;

		//=========RHI Resource====================
		virtual void* GetRHISwapChain() = 0;
		virtual void* GetRHIRenderTargetView() = 0;
		//=========================================

		//=========Misc============================
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		ERHIFormat GetPixelFormat() const { return m_PixelFormat; }
		//==========================================

	protected:
		uint32_t m_Width;
		uint32_t m_Height;
		ERHIFormat m_PixelFormat;
	};
}