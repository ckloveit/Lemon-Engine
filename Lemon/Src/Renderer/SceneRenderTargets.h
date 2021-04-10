#pragma once
#include "Core/Core.h"
#include "RHI/RHI.h"

namespace Lemon
{
	class RHICommandList;
	class SceneRenderer;
	class RHITexture2D;

	class LEMON_API SceneRenderTargets
	{
	public:
		SceneRenderTargets(uint32_t sizeX, uint32_t sizeY);
		~SceneRenderTargets();
	public:
		void Allocate(Ref<RHICommandList> RHICmdList);

		void OnResize(uint32_t newSizeX, uint32_t newSizeY);

		ERHIPixelFormat GetSceneColorFormat() const;
		ERHIPixelFormat GetSceneDepthFormat() const;
		Ref<RHITexture2D> GetSceneColorTexture() const { return m_SceneColorTex; }
		Ref<RHITexture2D> GetSceneDepthTexture() const { return m_SceneDepthTex; }

	private:
		Ref<RHITexture2D> m_SceneColorTex;
		Ref<RHITexture2D> m_SceneDepthTex;
		uint32_t m_SizeX;
		uint32_t m_SizeY;
	};
}