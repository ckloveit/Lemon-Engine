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
		Ref<RHITexture2D> GetSceneColorTexture() const { return m_SceneColorTex; }

	private:
		Ref<RHITexture2D> m_SceneColorTex;
		uint32_t m_SizeX;
		uint32_t m_SizeY;
	};
}