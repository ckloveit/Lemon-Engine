#pragma once
#include "Core/Core.h"
#include "D3D11DynamicRHI.h"
#include "RHI/RHICommandList.h"
#include <map>

#include <d3d11_4.h>
#include <DirectXMath.h>
using namespace DirectX;

namespace Lemon
{
	class Renderer;
	class LEMON_API D3D11CommandList : public RHICommandList
	{
	public:
		D3D11CommandList(D3D11DynamicRHI* D3D11RHI, Renderer* renderer);

		//===================================RHI RenderCommand Helper function==================================//
		virtual void RHIClearRenderTarget(Ref<RHITexture2D> renderTarget, glm::vec4 backgroundColor,
            Ref<RHITexture2D> depthStencilTarget = nullptr, float depthClear = 1.0f, float stencilClear = 0)  override;

		virtual void RHIClearRenderTarget(Ref<RHITextureCube> renderTargets, int renderTargetIndex, glm::vec4 backgroundColor,
			Ref<RHITexture2D> depthStencilTarget = nullptr, float depthClear = 1.0f, float stencilClear = 0)  override;


		virtual void SetRenderTarget(Ref<RHITexture2D> colorTarget,Ref<RHITexture2D> depthTarget = nullptr) override; 
		
		virtual void SetRenderTarget(Ref<RHITextureCube> colorTargets, int colorTargetIndex, Ref<RHITexture2D> depthTarget = nullptr) override;

		virtual void SetRenderTarget(Ref<RHISwapChain> swapChain) override;

		virtual void SetGraphicsPipelineState(const GraphicsPipelineStateInitializer& GraphicsPSOInit) override;

		virtual void SetViewport(const Viewport& viewport) override;

		virtual void SetIndexBuffer(const RHIIndexBuffer* indexBuffer) override;

		virtual void SetVertexBuffer(int streamIndex, const RHIVertexBuffer* vertexBuffer) override;

		virtual void DrawIndexPrimitive(uint32_t VertexOffset, uint32_t IndexOffset, uint32_t NumPrimitives, uint32_t FirstInstance = 0, uint32_t NumInstances = 1) override;

		virtual void Flush() override;
		
		virtual void SetUniformBuffer(uint32_t slot, EUniformBufferUsageScopeType scopeType, const RHIUniformBufferBaseRef& uniformBuffer) override;

		virtual void SetSamplerState(uint32_t slot, const Ref<RHISamplerState>& samplerState) override;
		
		virtual void SetTexture(uint32_t slot, const Ref<RHITexture>& texture) override;

		virtual void SetMipTexture(Ref<RHITextureCube> targetTex, int mipIndex, int mipWidth, int mipHeight, std::vector<Ref<RHITexture2D>> mipTextures) override;

		//=======================================================================================================//
	private:
		D3D11DynamicRHI* m_D3D11RHI;
		GraphicsPipelineStateInitializer m_CurrentGraphicsPipelineState;
		EPrimitiveType m_CurrentPrimitiveType;

		Ref<RHIRasterizerState> m_DefaultRasterizerState;
		Ref<RHIDepthStencilState> m_DefaultDepthStencilState;
	};
}