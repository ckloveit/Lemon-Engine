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
	struct VertexPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
	};

	class Renderer;
	class LEMON_API D3D11CommandList : public RHICommandList
	{
	public:
		D3D11CommandList(D3D11DynamicRHI* D3D11RHI, Renderer* renderer);

		//===================================RHI RenderCommand Helper function==================================//
		virtual void RHIClearRenderTarget(Ref<RHITexture2D> renderTarget, glm::vec4 backgroundColor) override;

		virtual void SetRenderTarget(Ref<RHITexture2D> colorTarget) override;

		virtual void SetRenderTarget(Ref<RHISwapChain> swapChain) override;

		virtual void SetGraphicsPipelineState(const GraphicsPipelineStateInitializer& GraphicsPSOInit) override;

		virtual void SetViewport(const Viewport& viewport) override;

		virtual void SetIndexBuffer(const RHIIndexBuffer* indexBuffer) override;

		virtual void SetVertexBuffer(int streamIndex, const RHIVertexBuffer* vertexBuffer) override;

		virtual void DrawIndexPrimitive(uint32_t VertexOffset, uint32_t IndexOffset, uint32_t NumPrimitives, uint32_t FirstInstance = 0, uint32_t NumInstances = 1) override;

		virtual void Flush() override;

		virtual void DebugRenderer(Ref<RHITexture2D> colorTarget) override;
		//=======================================================================================================//
	private:
		D3D11DynamicRHI* m_D3D11RHI;
		GraphicsPipelineStateInitializer m_CurrentGraphicsPipelineState;
		EPrimitiveType m_CurrentPrimitiveType;

		//
		ComPtr<ID3D11InputLayout> m_pVertexLayout;	// 顶点输入布局
		ComPtr<ID3D11Buffer> m_pVertexBuffer;		// 顶点缓冲区
		ComPtr<ID3D11VertexShader> m_pVertexShader;	// 顶点着色器
		ComPtr<ID3D11PixelShader> m_pPixelShader;	// 像素着色器
	};
}