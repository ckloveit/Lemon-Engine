#include "LemonPCH.h"
#include "PixelShaderUtils.h"
#include "Geometry/Quad.h"
#include "RHI/RHIStaticStates.h"
#include "GlobalRenderResources.h"

namespace Lemon
{
	void PixelShaderUtils::DrawFullScreenQuad(Ref<RHICommandList>& RHICmdList, 
		std::shared_ptr<RHIVertexDeclaration> VertexDeclaration,
		std::shared_ptr<RHIVertexShader> VertexShader, 
		std::shared_ptr<RHIPixelShader> PixelShader,
		std::vector<std::shared_ptr<RHITexture>> Textures /*= std::vector<std::shared_ptr<RHITexture>>()*/)
	{
		static std::shared_ptr<RHIBlendState> FullScreenBlendState = TStaticBlendStateWriteMask<>::CreateRHI();
		static std::shared_ptr<RHIDepthStencilState> FullScreenDepthState = TStaticDepthStencilState<false, Lemon::CF_Always>::CreateRHI();

		// Set PSO
		GraphicsPipelineStateInitializer PSOInit;
		PSOInit.BoundShaderState.VertexShaderRHI = VertexShader;
		PSOInit.BoundShaderState.PixelShaderRHI = PixelShader;
		PSOInit.BoundShaderState.VertexDeclarationRHI = VertexDeclaration;
		PSOInit.BlendState = FullScreenBlendState;
		PSOInit.PrimitiveType = EPrimitiveType::PT_TriangleList;
		PSOInit.DepthStencilState = FullScreenDepthState;
		PSOInit.RasterizerState = nullptr;// Use Default RasterizerState

		RHICmdList->SetGraphicsPipelineState(PSOInit);

		// Set VertexBuffer and IndexBuffer
		if (GlobalRenderResources* GlobalResouce = GlobalRenderResources::GetInstance())
		{
			RHICmdList->SetIndexBuffer(GlobalRenderResources::GetInstance()->FullScreenIndexBuffer);
			RHICmdList->SetVertexBuffer(0, GlobalRenderResources::GetInstance()->FullScreenVertexBuffer);
		}
		// Set Textures
		for (int i = 0; i < Textures.size(); i++)
		{
			RHICmdList->SetTexture(0, Textures[i]);
		}

		// Draw
		RHICmdList->DrawIndexPrimitive(0, 0, 2);
	}
}