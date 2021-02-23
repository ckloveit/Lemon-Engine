#include "LemonPCH.h"
#include "SceneRenderTargets.h"
#include "RHI/RHI.h"
#include "RHI/DynamicRHI.h"

namespace Lemon
{
	SceneRenderTargets::SceneRenderTargets(uint32_t sizeX, uint32_t sizeY)
		: m_SizeX(sizeX)
		, m_SizeY(sizeY)

	{

	}
	void SceneRenderTargets::Allocate(Ref<RHICommandList> RHICmdList)
	{
		RHIResourceCreateInfo createInfo;
		m_SceneColorTex = RHICreateTexture2D(m_SizeX, m_SizeY, GetSceneColorFormat(), 1, 
			RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable,
			createInfo);
	}
	SceneRenderTargets::~SceneRenderTargets()
	{

	}

	ERHIPixelFormat SceneRenderTargets::GetSceneColorFormat() const
	{
		ERHIPixelFormat SceneColorBufferFormat = RHI_PF_R16G16B16A16_Float;
		return SceneColorBufferFormat;
	}

}