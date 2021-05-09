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

		virtual void* GetNativeResource() const { return nullptr; }
		
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

	private:
		uint32_t m_Size;
		// e.g. RHI_BUF_UnorderedAccess
		uint32_t m_Usage;
	};

	class RHIUniformBufferBase : public RHIResource
	{
	public:
		RHIUniformBufferBase(uint32_t size)
			:m_Size(size)
		{}
		// @return we want Update Buffer address
		virtual void* Lock() const = 0;
		// close the Update Buffer address
		virtual bool UnLock() const = 0;

	private:
		uint32_t m_Size;
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
		ERHIPixelFormat GetPixelFormat() const { return m_PixelFormat; }
		uint32_t GetNumMip() const { return m_NumMips; }

		//=========Dynamic cast methods=============
		virtual class RHITexture2D* GetTexture2D() { return nullptr; }
		virtual class RHITextureCube* GetTextureCube() { return nullptr; }


		//=========================================


		//=========RHI Resource====================
		virtual void* GetNativeResource() = 0;
		virtual void* GetNativeShaderResourceView() = 0;
		virtual void* GetNativeRenderTargetView(int index) = 0;
		virtual void* GetNativeDepthStencilView() = 0;
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

	class LEMON_API RHITextureCube : public RHITexture
	{
	public:
		/** Initialization constructor. */
		RHITextureCube(uint32_t sizeX, uint32_t sizeY, uint32_t numMips, ERHIPixelFormat textureFormat)
			: RHITexture(numMips, textureFormat)
			, m_SizeX(sizeX)
			, m_SizeY(sizeY)
		{}
		// ====Dynamic cast methods.
		virtual RHITextureCube* GetTextureCube() override { return this; }

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
	// RasterizerState
	// ====State==========//
	class RHISamplerState : public RHIResource
	{
	public:
		RHISamplerState(const SamplerStateInitializer& Init)
			:m_SamplerStateInitializer(Init)
		{
			
		}
	protected:
		SamplerStateInitializer m_SamplerStateInitializer;
	};

	class RHIRasterizerState : public RHIResource
	{
	public:
		RHIRasterizerState(const RasterizerStateInitializer& Init)
			: m_RasterizerInitializer(Init){ }

	protected:
		RasterizerStateInitializer m_RasterizerInitializer;
	};
	
	class RHIBlendState : public RHIResource
	{
	public:
		RHIBlendState(const BlendStateInitializer& Init)
            : m_BlendStateInitializer(Init){ }

	protected:
		BlendStateInitializer m_BlendStateInitializer;
	};
	
	class DepthStencilOperation
	{
	public:
		enum Type
		{
			// don't use those directly, use the combined versions below
			// 4 bits are used for depth and 4 for stencil to make the hex value readable and non overlapping
			DepthNop =		0x00,
            DepthRead =		0x01,
            DepthWrite =	0x02,
            DepthMask =		0x0f,
            StencilNop =	0x00,
            StencilRead =	0x10,
            StencilWrite =	0x20,
            StencilMask =	0xf0,

            // use those:
            DepthNop_StencilNop = DepthNop + StencilNop,
            DepthRead_StencilNop = DepthRead + StencilNop,
            DepthWrite_StencilNop = DepthWrite + StencilNop,
            DepthNop_StencilRead = DepthNop + StencilRead,
            DepthRead_StencilRead = DepthRead + StencilRead,
            DepthWrite_StencilRead = DepthWrite + StencilRead,
            DepthNop_StencilWrite = DepthNop + StencilWrite,
            DepthRead_StencilWrite = DepthRead + StencilWrite,
            DepthWrite_StencilWrite = DepthWrite + StencilWrite,
        };

	private:
		Type Value;
	public:
		// constructor
		DepthStencilOperation(Type InValue = DepthNop_StencilNop)
            : Value(InValue)
		{
		}
		bool operator==(const DepthStencilOperation& rhs) const
		{
			return Value == rhs.Value;
		}
		bool operator != (const DepthStencilOperation& RHS) const
		{
			return Value != RHS.Value;
		}
		inline bool IsUsingDepthStencil() const
		{
			return Value != DepthNop_StencilNop;
		}
		inline bool IsUsingDepth() const
		{
			return (ExtractDepth() != DepthNop);
		}
		inline bool IsUsingStencil() const
		{
			return (ExtractStencil() != StencilNop);
		}
		inline bool IsDepthWrite() const
		{
			return ExtractDepth() == DepthWrite;
		}
		inline bool IsDepthRead() const
		{
			return ExtractDepth() == DepthRead;
		}
		inline bool IsStencilWrite() const
		{
			return ExtractStencil() == StencilWrite;
		}
		inline bool IsStencilRead() const
		{
			return ExtractStencil() == StencilRead;
		}
		inline bool IsAnyWrite() const
		{
			return IsDepthWrite() || IsStencilWrite();
		}

		inline void SetDepthWrite()
		{
			Value = (Type)(ExtractStencil() | DepthWrite);
		}
		inline void SetStencilWrite()
		{
			Value = (Type)(ExtractDepth() | StencilWrite);
		}
		inline void SetDepthStencilWrite(bool bDepth, bool bStencil)
		{
			Value = DepthNop_StencilNop;

			if (bDepth)
			{
				SetDepthWrite();
			}
			if (bStencil)
			{
				SetStencilWrite();
			}
		}
		
	private:
		inline Type ExtractDepth() const
		{
			return (Type)(Value & DepthMask);
		}
		inline Type ExtractStencil() const
		{
			return (Type)(Value & StencilMask);
		}
	};
	
	class RHIDepthStencilState : public RHIResource
	{
	public:
		RHIDepthStencilState(const DepthStencilStateInitializer& Init)
			:m_DepthStencilStateInitializer(Init){ }

	protected:
		DepthStencilStateInitializer m_DepthStencilStateInitializer;
	};
	
	//==========Pipeline state===================================//
	struct GraphicsPipelineStateInitializer
	{
	public:
		RHIBoundShaderStateInput			BoundShaderState;
		EPrimitiveType					PrimitiveType;
		Ref<RHIBlendState>				BlendState;
		Ref<RHIRasterizerState>			RasterizerState;
		Ref<RHIDepthStencilState>		DepthStencilState;

		bool IsValid() const 
		{
			return BoundShaderState.VertexDeclarationRHI && BoundShaderState.VertexShaderRHI;
		}

		bool operator==(const GraphicsPipelineStateInitializer& rhs) const 
		{
			if (BoundShaderState.PixelShaderRHI == rhs.BoundShaderState.PixelShaderRHI &&
				BoundShaderState.VertexShaderRHI == rhs.BoundShaderState.VertexShaderRHI &&
				BoundShaderState.VertexDeclarationRHI == rhs.BoundShaderState.VertexDeclarationRHI &&
				BlendState == rhs.BlendState &&
				RasterizerState == rhs.RasterizerState &&
				DepthStencilState == rhs.DepthStencilState)
			{
				return true;
			}
			return false;
		}
		bool operator!=(const GraphicsPipelineStateInitializer& rhs) const 
		{
			return !(*this == rhs);
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


	//
	typedef Ref<RHIIndexBuffer> RHIIndexBufferRef;
	typedef Ref<RHIVertexBuffer> RHIVertexBufferRef;
	typedef Ref<RHIUniformBufferBase> RHIUniformBufferBaseRef;

	
}