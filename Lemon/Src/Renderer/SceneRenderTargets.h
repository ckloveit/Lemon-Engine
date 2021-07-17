#pragma once
#include "Core/Core.h"
#include "RHI/RHI.h"
#include "Core/TSingleon.h"
namespace Lemon
{
	class RHICommandList;
	class SceneRenderer;
	class RHITexture2D;
	class Renderer;

	class LEMON_API SceneRenderTargets : public TSingleton<SceneRenderTargets>
	{
	public:
		SceneRenderTargets(uint32_t sizeX, uint32_t sizeY);
		~SceneRenderTargets();
	public:
		void Allocate(Ref<RHICommandList> RHICmdList, Renderer* renderer);

		void OnResize(uint32_t newSizeX, uint32_t newSizeY, Renderer* renderer);

		ERHIPixelFormat GetSceneColorFormat() const;
		ERHIPixelFormat GetSceneDepthFormat() const;
		Ref<RHITexture2D> GetSceneColorTexture() const { return m_SceneColorTex; }
		Ref<RHITexture2D> GetSceneDepthTexture() const { return m_SceneDepthTex; }

	public:
		Ref<RHITexture2D> m_SceneColorTex;
		Ref<RHITexture2D> m_SceneDepthTex;

		//Deferred Shading
		Ref<RHITexture2D> m_GBufferPosition;// Need Remove in future
		Ref<RHITexture2D> m_GBufferAlbedo;
		Ref<RHITexture2D> m_GBufferNormal;
		Ref<RHITexture2D> m_GBufferMaterial;

	private:
		uint32_t m_SizeX;
		uint32_t m_SizeY;
	};
}