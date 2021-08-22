#include "LemonPCH.h"
#include "SceneRenderTargets.h"
#include "RHI/RHI.h"
#include "RHI/DynamicRHI.h"
#include "Renderer.h"

namespace Lemon
{
	SceneRenderTargets::SceneRenderTargets(uint32_t sizeX, uint32_t sizeY)
		: Super()
		, m_SizeX(sizeX)
		, m_SizeY(sizeY)

	{

	}
	void SceneRenderTargets::Allocate(Ref<RHICommandList> RHICmdList, Renderer* renderer)
	{
		RHIResourceCreateInfo createInfo;
		m_SceneColorTex = RHICreateTexture2D(m_SizeX, m_SizeY, GetSceneColorFormat(), 1, 
			RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo);
		m_SceneDepthTex = RHICreateTexture2D(m_SizeX, m_SizeY, GetSceneDepthFormat(), 1,
			RHI_TexCreate_ShaderResource | RHI_TexCreate_DepthStencilTargetable, createInfo);
		if (renderer->GetShadingPath() == EShadingPath::Deferred)
		{
			m_GBufferPosition = RHICreateTexture2D(m_SizeX, m_SizeY, ERHIPixelFormat::RHI_PF_R16G16B16A16_Float, 1,
				RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo);
			m_GBufferNormal = RHICreateTexture2D(m_SizeX, m_SizeY, ERHIPixelFormat::RHI_PF_R16G16B16A16_Float, 1,
				RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo);
			m_GBufferAlbedo = RHICreateTexture2D(m_SizeX, m_SizeY, ERHIPixelFormat::RHI_PF_R16G16B16A16_Float, 1,
				RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo);
			m_GBufferMaterial = RHICreateTexture2D(m_SizeX, m_SizeY, ERHIPixelFormat::RHI_PF_R16G16B16A16_Float, 1,
				RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo);
		}

	}
	
	void SceneRenderTargets::OnResize(uint32_t newSizeX, uint32_t newSizeY, Renderer* renderer)
	{
		if (m_SizeX == newSizeX && m_SizeY == newSizeY)
		{
			return;
		}

		m_SizeX = newSizeX;
		m_SizeY = newSizeY;
		
		//Recreate texture
		RHIResourceCreateInfo createInfo;
		m_SceneColorTex = RHICreateTexture2D(m_SizeX, m_SizeY, GetSceneColorFormat(), 1, 
            RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable,
            createInfo);
		m_SceneDepthTex = RHICreateTexture2D(m_SizeX, m_SizeY, GetSceneDepthFormat(), 1,
                RHI_TexCreate_ShaderResource | RHI_TexCreate_DepthStencilTargetable,
                createInfo);

		if (renderer->GetShadingPath() == EShadingPath::Deferred)
		{
			m_GBufferPosition = RHICreateTexture2D(m_SizeX, m_SizeY, ERHIPixelFormat::RHI_PF_R16G16B16A16_Float, 1,
				RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo);
			m_GBufferNormal = RHICreateTexture2D(m_SizeX, m_SizeY, ERHIPixelFormat::RHI_PF_R16G16B16A16_Float, 1,
				RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo);
			m_GBufferAlbedo = RHICreateTexture2D(m_SizeX, m_SizeY, ERHIPixelFormat::RHI_PF_R16G16B16A16_Float, 1,
				RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo);
			m_GBufferMaterial = RHICreateTexture2D(m_SizeX, m_SizeY, ERHIPixelFormat::RHI_PF_R16G16B16A16_Float, 1,
				RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo);
		}

	}
	SceneRenderTargets::~SceneRenderTargets()
	{

	}

	ERHIPixelFormat SceneRenderTargets::GetSceneColorFormat() const
	{
		const ERHIPixelFormat SceneColorBufferFormat = RHI_PF_R16G16B16A16_Float;
		return SceneColorBufferFormat;
	}

	ERHIPixelFormat SceneRenderTargets::GetSceneDepthFormat() const
	{
		const ERHIPixelFormat SceneDepthBufferFormat = RHI_PF_D32_Float;
		return SceneDepthBufferFormat;
	}
}