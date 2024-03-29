#pragma once
#include "Core/Core.h"
#include "RHI.h"
#include "glm/glm.hpp"
#include "RenderCore/Viewport.h"
#include "RHIResources.h"

namespace Lemon
{
	class Renderer;
	class RHITexture2D;
	class RHIIndexBuffer;
	class RHIVertexBuffer;

	class LEMON_API RHICommandList
	{
	public:
		RHICommandList(Renderer* renderer);
		virtual ~RHICommandList() {}
		//=====Static function====================
		static RHICommandList& Get();


		//===================================RHI RenderCommand Helper function==================================//
		virtual void RHIClearRenderTarget(Ref<RHITexture2D> renderTarget, glm::vec4 backgroundColor,
			Ref<RHITexture2D> depthStencilTarget = nullptr, float depthClear = 1.0f, float stencilClear = 0) = 0;
		virtual void RHIClearRenderTarget(Ref<RHITextureCube> renderTargets, int renderTargetIndex, glm::vec4 backgroundColor,
			Ref<RHITexture2D> depthStencilTarget = nullptr, float depthClear = 1.0f, float stencilClear = 0) = 0;
		virtual void RHIClearRenderTarget(std::vector<Ref<RHITexture2D>> colorTargets, glm::vec4 backgroundColor,
			Ref<RHITexture2D> depthStencilTarget = nullptr, float depthClear = 1.0f, float stencilClear = 0) = 0;

		virtual void SetRenderTarget(Ref<RHITexture2D> colorTarget, Ref<RHITexture2D> depthTarget = nullptr) = 0;
		virtual void SetRenderTarget(Ref<RHITextureCube> colorTargets, int colorTargetIndex, Ref<RHITexture2D> depthTarget = nullptr) = 0;
		virtual void SetRenderTarget(std::vector<Ref<RHITexture2D>> colorTargets, Ref<RHITexture2D> depthTarget = nullptr) = 0;

		virtual void SetRenderTarget(Ref<RHISwapChain> swapChain, float depthClear = 1.0f, float stencilClear = 0) = 0;

		virtual void SetGraphicsPipelineState(const GraphicsPipelineStateInitializer& GraphicsPSOInit) = 0;

		virtual void SetViewport(const Viewport& viewport) = 0;

		virtual void SetIndexBuffer(const Ref<RHIIndexBuffer>& indexBuffer) { SetIndexBuffer(indexBuffer.get()); }
		virtual void SetIndexBuffer(const RHIIndexBuffer* indexBuffer) = 0;

		virtual void SetVertexBuffer(int streamIndex, const Ref<RHIVertexBuffer>& vertexBuffer) { SetVertexBuffer(streamIndex, vertexBuffer.get()); }
		virtual void SetVertexBuffer(int streamIndex, const RHIVertexBuffer* vertexBuffer) = 0;

		virtual void DrawIndexPrimitive(uint32_t VertexOffset, uint32_t IndexOffset, uint32_t NumPrimitives, uint32_t FirstInstance = 0, uint32_t NumInstances = 1) = 0;

		virtual void Flush() = 0;

		// ConstantBuffer
		virtual void SetUniformBuffer(uint32_t slot, EUniformBufferUsageScopeType scopeType, const RHIUniformBufferBaseRef& uniformBuffer) = 0;

		// SamplerState
		virtual void SetSamplerState(uint32_t slot,const Ref<RHISamplerState>& samplerState) = 0;

		// Texture
		virtual void SetTexture(uint32_t slot, const Ref<RHITexture>& texture) = 0;	
		
		virtual void SetMipTexture(Ref<RHITextureCube> targetTex, int mipIndex, int mipWidth, int mipHeight, std::vector<Ref<RHITexture2D>> mipTextures) = 0;

		//=======================================================================================================//
	private:
		static RHICommandList* s_Instance;
	};

}