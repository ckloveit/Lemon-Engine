#pragma once
#include "Core/Core.h"
#include "RHI.h"
#include <string>

namespace Lemon
{
	class LEMON_API RHITexture
	{
	public:
		RHITexture(uint32_t mumMips, ERHIPixelFormat textureFormat)
			:m_NumMips(mumMips)
			, m_PixelFormat(textureFormat)
		{}

		virtual ~RHITexture() {};

		const std::string& GetName() const { return m_TextureName; }
		void SetName(const std::string& name) { m_TextureName = name; }

		//=========Dynamic cast methods=============
		virtual class RHITexture2D* GetTexture2D() { return nullptr; }


		//=========================================


		//=========RHI Resource====================
		virtual void* GetNativeResource() = 0;
		virtual void* GetNativeShaderResourceView() = 0;
		virtual void* GetNativeRenderTargetView() = 0;
		//=========================================

	protected:
		uint32_t m_NumMips;
		ERHIPixelFormat m_PixelFormat;
		std::string m_TextureName;
	};

	class LEMON_API RHITexture2D : public RHITexture
	{
	public:
		/** Initialization constructor. */
		RHITexture2D(uint32_t sizeX, uint32_t sizeY, uint32_t numMips, ERHIPixelFormat textureFormat)
			: RHITexture(numMips, textureFormat)
			, m_SizeX(sizeX)
			, m_SizeY(sizeY)
		{}
		// ====Dynamic cast methods.
		virtual RHITexture2D* GetTexture2D() override { return this; }

		uint32_t GetSizeX() const { return m_SizeX; }
		uint32_t GetSizeY() const { return m_SizeY; }

	private:
		uint32_t m_SizeX;
		uint32_t m_SizeY;
	};

}