#pragma once
#include "Core/Core.h"
#include "RHI/RHI.h"
#include "RHI/RHITexture.h"
#include "RHI/D3D11/D3D11RHI.h"
#include "RHI/D3D11/D3D11DynamicRHI.h"

namespace Lemon
{
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
		//=========================================
	};



}

