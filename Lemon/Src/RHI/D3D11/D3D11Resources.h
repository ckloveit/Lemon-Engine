#pragma once
#include "Core/Core.h"
#include "RHI/RHI.h"
#include "RHI/RHIResources.h"
#include "RHI/D3D11/D3D11RHI.h"
#include "RHI/D3D11/D3D11DynamicRHI.h"
#include <array>
namespace Lemon
{
	//==========================Textures==========================//
	class D3D11TextureBase
	{
	public:
		D3D11TextureBase(D3D11DynamicRHI* D3D11RHI, ID3D11Resource* resource,
			ID3D11ShaderResourceView* shaderResourceView,
			const std::vector<ID3D11RenderTargetView*>& renderTargetViews,
			ID3D11DepthStencilView* depthStencilView)
			: m_D3DRHI(D3D11RHI)
			, m_Resource(resource)
			, m_ShaderResourceView(shaderResourceView)
			, m_RenderTargetViews(renderTargetViews)
			, m_DepthStencilView(depthStencilView)
		{

		}
		virtual ~D3D11TextureBase()
		{
			D3D11::SafeRelease(m_Resource);
			D3D11::SafeRelease(m_ShaderResourceView);
			for (int i = 0; i < m_RenderTargetViews.size(); i++)
			{
				D3D11::SafeRelease(m_RenderTargetViews[i]);
			}
			D3D11::SafeRelease(m_DepthStencilView);
		}

		uint32_t GetMemorySize() const { return m_MemorySize; }
		void SetMemorySize(uint32_t memorySize) { m_MemorySize = memorySize; }


		//============ Accessors.=======================
		ID3D11Resource* GetResource() const { return m_Resource; }
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_ShaderResourceView; }
		const std::vector<ID3D11RenderTargetView*>& GetRenderTargetViews() const { return m_RenderTargetViews; }
		ID3D11DepthStencilView* GetDepthStencilView() const { return m_DepthStencilView; }
		//==============================================


	protected:
		/** Amount of memory allocated by this texture, in bytes. */
		uint32_t m_MemorySize;

		/** The D3D11 RHI that created this texture. */
		D3D11DynamicRHI* m_D3DRHI;

		ID3D11Resource* m_Resource;
		ID3D11ShaderResourceView* m_ShaderResourceView;
		std::vector<ID3D11RenderTargetView*> m_RenderTargetViews;
		ID3D11DepthStencilView* m_DepthStencilView;


	};

	class D3D11Texture2D : public RHITexture2D, public D3D11TextureBase
	{
	public:
		D3D11Texture2D(class D3D11DynamicRHI* D3D11RHI,
			ID3D11Texture2D* resource,
			ID3D11ShaderResourceView* shaderResourceView,
			const std::vector<ID3D11RenderTargetView*>& renderTargetViews,
			ID3D11DepthStencilView* depthStencilView,
			uint32_t sizeX, uint32_t sizeY, uint32_t numMips, ERHIPixelFormat textureFormat)
			: RHITexture2D(sizeX, sizeY, numMips, textureFormat)
			, D3D11TextureBase(D3D11RHI, resource, shaderResourceView, renderTargetViews, depthStencilView)
		{

		}

		//=========RHI Resource====================
		virtual void* GetNativeResource() override final { return GetResource(); }

		virtual void* GetNativeShaderResourceView() override final { return GetShaderResourceView(); }

		virtual void* GetNativeRenderTargetView() override final { return GetRenderTargetViews().size() ? GetRenderTargetViews()[0] : nullptr; }

		virtual void* GetNativeDepthStencilView() override final { return GetDepthStencilView();};
		//=========================================
	};

	class D3D11TextureCube : public RHITextureCube, public D3D11TextureBase
	{
	public:
		D3D11TextureCube(class D3D11DynamicRHI* D3D11RHI,
			ID3D11Texture2D* resource,
			ID3D11ShaderResourceView* shaderResourceView,
			const std::vector<ID3D11RenderTargetView*>& renderTargetViews,
			ID3D11DepthStencilView* depthStencilView,
			uint32_t sizeX, uint32_t sizeY, uint32_t numMips, ERHIPixelFormat textureFormat)
			: RHITextureCube(sizeX, sizeY, numMips, textureFormat)
			, D3D11TextureBase(D3D11RHI, resource, shaderResourceView, renderTargetViews, depthStencilView)
		{

		}

		//=========RHI Resource====================
		virtual void* GetNativeResource() override final { return GetResource(); }

		virtual void* GetNativeShaderResourceView() override final { return GetShaderResourceView(); }

		virtual void* GetNativeRenderTargetView() override final { return GetRenderTargetViews().size() ? GetRenderTargetViews()[0] : nullptr; }

		virtual void* GetNativeDepthStencilView() override final { return GetDepthStencilView(); };
		//=========================================
	};


	//===========================================================//


	//==========================Shaders==========================//
	class D3D11VertexShader : public RHIVertexShader
	{
	public:
		D3D11VertexShader(ID3D11VertexShader* Resource, ID3D10Blob* ShaderBlob);
		virtual ~D3D11VertexShader();

	public:
		ID3D11VertexShader* m_Resource;

		/** The vertex shader's bytecode, with custom data attached. */
		ID3D10Blob* m_ShaderBlob;
	};

	class D3D11PixelShader : public RHIPixelShader
	{
	public:
		D3D11PixelShader(ID3D11PixelShader* Resource);
		virtual ~D3D11PixelShader();

	public:
		ID3D11PixelShader* m_Resource;
	};
	//===========================================================//


	//==========================Buffers==========================//
	class D3D11IndexBuffer : public RHIIndexBuffer
	{
	public:
		D3D11IndexBuffer(D3D11DynamicRHI* D3D11RHI, ID3D11Buffer* buffer, uint32_t size, uint32_t usage)
			: RHIIndexBuffer(size, usage)
			, m_D3DRHI(D3D11RHI)
			, m_Buffer(buffer)
		{}
		virtual ~D3D11IndexBuffer();

		// @return we want Update Buffer address
		virtual void* Lock() const override;
		// close the Update Buffer address
		virtual bool UnLock() const override;

		virtual void* GetNativeResource() const override { return m_Buffer; }
	private:
		ID3D11Buffer* m_Buffer;
		D3D11DynamicRHI* m_D3DRHI;
	};

	class D3D11VertexBuffer : public RHIVertexBuffer
	{
	public:
		D3D11VertexBuffer(D3D11DynamicRHI* D3D11RHI, ID3D11Buffer* buffer, uint32_t size, uint32_t usage)
			: RHIVertexBuffer(size, usage)
			, m_D3DRHI(D3D11RHI)
			, m_Buffer(buffer)
		{}
		virtual ~D3D11VertexBuffer();

		// @return we want Update Buffer address
		virtual void* Lock() const override;
		// close the Update Buffer address
		virtual bool UnLock() const override;

		virtual void* GetNativeResource() const override { return m_Buffer; }
	private:
		ID3D11Buffer* m_Buffer;
		D3D11DynamicRHI* m_D3DRHI;
	};

	class D3D11UniformBuffer : public RHIUniformBufferBase
	{
	public:
		D3D11UniformBuffer(D3D11DynamicRHI* D3D11RHI, ID3D11Buffer* buffer, uint32_t size)
		: RHIUniformBufferBase(size)
		, m_D3DRHI(D3D11RHI)
        , m_Buffer(buffer)
		{
			
		}
		virtual ~D3D11UniformBuffer();

		// @return we want Update Buffer address
		virtual void* Lock() const override;
		// close the Update Buffer address
		virtual bool UnLock() const override;
	
		virtual void* GetNativeResource() const override { return m_Buffer; }
	private:
		ID3D11Buffer* m_Buffer;
		D3D11DynamicRHI* m_D3DRHI;
	};
	//===========================================================//


	//==========================Shader Binds==========================//
	/** Convenience typedef: preallocated array of D3D11 input element descriptions. */
	typedef std::vector<D3D11_INPUT_ELEMENT_DESC> D3D11VertexElements;

	class D3D11VertexDeclaration : public RHIVertexDeclaration
	{
	public:
		/** Initialization constructor. */
		explicit D3D11VertexDeclaration(ID3D11Device* Direct3DDevice, Ref<RHIVertexShader> vertexShader, const D3D11VertexElements& InElements, const uint16_t* InStrides);

		~D3D11VertexDeclaration();

		/** Elements of the vertex declaration. */
		D3D11VertexElements m_VertexElements;

		ID3D11InputLayout* m_InputLayout;
	};

	/**
	 * Combined shader state and vertex definition for rendering geometry.
	 * Each unique instance consists of a vertex decl, vertex shader, and pixel shader.
	 */
	//class D3D11BoundShaderState : public RHIBoundShaderState
	//{
	//public:
	//	uint16_t m_StreamStrides[MaxVertexElementCount];
	//	ID3D11InputLayout* m_InputLayout;
	//	ID3D11VertexShader* m_VertexShader;
	//	ID3D11PixelShader* m_PixelShader;
	//	//ID3D11HullShader* m_HullShader;
	//	//ID3D11DomainShader* m_DomainShader;
	//	//ID3D11GeometryShader* m_GeometryShader;

	//	D3D11BoundShaderState(
	//		Ref<RHIVertexDeclaration> InVertexDeclarationRHI,
	//		Ref<RHIVertexShader> InVertexShaderRHI,
	//		Ref<RHIPixelShader> InPixelShaderRHI,
	//		ID3D11Device* Direct3DDevice
	//	);
	//	virtual ~D3D11BoundShaderState();

	//	/**
	//	 * Get the shader for the given frequency.
	//	 */
	//	FORCEINLINE ID3D11VertexShader* GetVertexShader() const { return m_VertexShader; }
	//	FORCEINLINE ID3D11PixelShader* GetPixelShader() const { return m_PixelShader; }
	//};
	//===========================================================//

	//==================PipelineStates===========================//
	

	//==============Utils============================================
	template<class T>
	struct TD3D11ResourceTraits
	{
	};
	template<>
	struct TD3D11ResourceTraits<RHIVertexDeclaration>
	{
		typedef D3D11VertexDeclaration TConcreteType;
	};
	template<>
	struct TD3D11ResourceTraits<RHIVertexShader>
	{
		typedef D3D11VertexShader TConcreteType;
	};
	template<>
	struct TD3D11ResourceTraits<RHIPixelShader>
	{
		typedef D3D11PixelShader TConcreteType;
	};
	//template<>
	//struct TD3D11ResourceTraits<RHIBoundShaderState>
	//{
	//	typedef D3D11BoundShaderState TConcreteType;
	//};

	template<typename TRHIType>
	static typename TD3D11ResourceTraits<TRHIType>::TConcreteType* D3D11ResourceCast(TRHIType* Resource)
	{
		return static_cast<typename TD3D11ResourceTraits<TRHIType>::TConcreteType*>(Resource);
	}
}

