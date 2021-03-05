#pragma once
#include "Core/Core.h"
#include "RHI.h"
#include "RHIDefinitions.h"
#include <string>
#include <array>

namespace Lemon
{
	FORCE_INLINE bool IsStaticUsage(uint32_t usage) { return usage & BUF_Static; }
	FORCE_INLINE bool IsDynamicUsage(uint32_t usage) { return usage & BUF_Dynamic; }

	class RHIResource
	{
	public:
		virtual ~RHIResource() {}


	};
	//
	// Buffers
	//	

	class LEMON_API RHIVertexBuffer : public RHIResource
	{
	public:
		/**
		 * Initialization constructor.
		 * @apram InUsage e.g. RHI_BUF_Dynamic
		 */
		RHIVertexBuffer(uint32_t size, uint32_t usage)
			: m_Size(size)
			, m_Usage(usage)
		{}
		// @return The number of bytes in the vertex buffer.
		uint32_t GetSize() const { return m_Size; }

		// @return The usage flags used to create the vertex buffer. e.g. RHI_BUF_Dynamic
		uint32_t GetUsage() const { return m_Usage; }

		// @return we want Update Buffer address
		virtual void* Lock() const = 0;
		// close the Update Buffer address
		virtual bool UnLock() const = 0;

		virtual void* GetNativeResource() const = 0;
	private:
		uint32_t m_Size;
		// e.g. RHI_BUF_UnorderedAccess
		uint32_t m_Usage;
	};

	class LEMON_API RHIIndexBuffer : public RHIResource
	{
	public:
		/**
		 * Initialization constructor.
		 * @apram InUsage e.g. RHI_BUF_Dynamic
		 */
		RHIIndexBuffer(uint32_t size, uint32_t usage)
			: m_Size(size)
			, m_Usage(usage)
		{}

		// @return The number of bytes in the vertex buffer.
		uint32_t GetSize() const { return m_Size; }
		// @return The usage flags used to create the vertex buffer. e.g. RHI_BUF_Dynamic
		uint32_t GetUsage() const { return m_Usage; }

		// @return we want Update Buffer address
		virtual void* Lock() const = 0;
		// close the Update Buffer address
		virtual bool UnLock() const = 0;

		virtual void* GetNativeResource() const = 0;

	private:
		uint32_t m_Size;
		// e.g. RHI_BUF_UnorderedAccess
		uint32_t m_Usage;
	};

	//
	// Shaders
	//
	class RHIShader : public RHIResource
	{
	public:
		explicit RHIShader(EShaderFrequency frequency)
			: m_Frequency(frequency)
		{
		}
		virtual ~RHIShader() {}
		inline EShaderFrequency GetFrequency() const
		{
			return m_Frequency;
		}
	protected:
		EShaderFrequency m_Frequency;
	};

	class LEMON_API RHIGraphicsShader : public RHIShader
	{
	public:
		explicit RHIGraphicsShader(EShaderFrequency frequency) : RHIShader(frequency) {}
	};

	class RHIVertexShader : public RHIGraphicsShader
	{
	public:
		RHIVertexShader() : RHIGraphicsShader(SF_Vertex) {}
	};

	class RHIPixelShader : public RHIGraphicsShader
	{
	public:
		RHIPixelShader() : RHIGraphicsShader(SF_Pixel) {}
	};


	//
	// Textures
	//
	class LEMON_API RHITexture : public RHIResource
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

	//==========================Textures==========================//
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
	//===========================================================//

	//
	// Shader bindings
	//

	class RHIVertexDeclaration : public RHIResource
	{
	public:
		virtual bool GetInitializer(VertexDeclarationElementList& Init) { return false; }

		uint16_t m_StreamStrides[MaxVertexElementCount];
	};

	struct RHIBoundShaderStateInput
	{
		Ref<RHIVertexDeclaration> VertexDeclarationRHI = nullptr;
		Ref<RHIVertexShader> VertexShaderRHI = nullptr;
		Ref<RHIPixelShader> PixelShaderRHI = nullptr;
	};

	//class RHIBoundShaderState : public RHIResource
	//{
	//public:
	//	RHIBoundShaderStateInput m_BoundStates;

	//};



	//==========Pipeline state===================================//
	struct GraphicsPipelineStateInitializer
	{
	public:
		RHIBoundShaderStateInput			BoundShaderState;
		EPrimitiveType					PrimitiveType;

		bool IsValid() const 
		{
			return BoundShaderState.VertexDeclarationRHI && BoundShaderState.VertexShaderRHI;
		}

		bool operator==(const GraphicsPipelineStateInitializer& rhs) const 
		{
			if (BoundShaderState.PixelShaderRHI == rhs.BoundShaderState.PixelShaderRHI ||
				BoundShaderState.VertexShaderRHI == rhs.BoundShaderState.VertexShaderRHI ||
				BoundShaderState.VertexDeclarationRHI == rhs.BoundShaderState.VertexDeclarationRHI)
			{
				return false;
			}
			return true;
		}
	};

	class RHIGraphicsPipelineState : RHIResource
	{
	public:
		RHIGraphicsPipelineState(const GraphicsPipelineStateInitializer& Initializer)
			: m_PSOInitializer(Initializer)
		{}
	public:
		GraphicsPipelineStateInitializer m_PSOInitializer;
	};

	

}