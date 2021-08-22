#include "LemonPCH.h"
#include "DeferredShadingRenderer.h"
#include "Renderer.h"
#include "Core/Engine.h"
#include "World/World.h"
#include "SceneShaderMap.h"
#include "SceneRenderStates.h"
#include "SceneRenderTargets.h"
#include "Core/TSingleon.h"
#include "RenderCore/PixelShaderUtils.h"

//------------------Components------------------//
#include "World/Components/TransformComponent.h"
#include "World/Components/StaticMeshComponent.h"

namespace Lemon
{

	void DeferredShadingRenderer::Render(Ref<RHICommandList> RHICmdList)
	{
		// Pre Depth Pass
		PreDepthPass(RHICmdList);
		// GBuffer Geometry Pass
		GBufferGeometryPass(RHICmdList);
		// GBuffer Lighting Pass
		GBufferLightingPass(RHICmdList);

	}
	void DeferredShadingRenderer::PreDepthPass(Ref<RHICommandList> RHICmdList)
	{
		if (!SceneRenderStates::Get() || !SceneShaderMap::Get())
			return;

		// Set Render Target
		RHICmdList->SetViewport(m_ViewInfo.ViewSize);
		Renderer* Render = Renderer::Get();
		RHICmdList->SetRenderTarget(Render->GetSceneRenderTargets()->GetSceneColorTexture(), Render->GetSceneRenderTargets()->GetSceneDepthTexture());
		Entity mainCameraEntity = Render->GetEngine()->GetSystem<World>()->GetMainCamera();
		RHICmdList->RHIClearRenderTarget(Render->GetSceneRenderTargets()->GetSceneColorTexture(), glm::vec4(0.1f, 0.4f, 0.7f, 1.0f),
			Render->GetSceneRenderTargets()->GetSceneDepthTexture());

		// Update View UniformBuffer
		Renderer::UpdateViewUniformBuffer(RHICmdList, mainCameraEntity);

		GraphicsPipelineStateInitializer PSOInit;
		PSOInit.BoundShaderState.VertexDeclarationRHI = SceneShaderMap::Get()->m_DepthOnlyDeclaration;
		PSOInit.BoundShaderState.VertexShaderRHI = SceneShaderMap::Get()->m_DepthOnlyVS;
		PSOInit.BoundShaderState.PixelShaderRHI = SceneShaderMap::Get()->m_DepthOnlyPS;

		PSOInit.PrimitiveType = EPrimitiveType::PT_TriangleList;
		//PSOInit.BlendState = SceneRenderStates::Get()->;
		//PSOInit.RasterizerState = PSOInitializer.RasterizerState;
		//PSOInit.DepthStencilState = SceneRenderStates::Get()->LessEqualWriteDepthStencilState;


		/*
			consider EnvironmentEntity to snap transform to camera transform
		*/
		for (int i = 0; i < Render->environmentEntitys.size(); i++)
		{
			Entity& envEntity = Render->environmentEntitys[i];
			envEntity.GetComponent<TransformComponent>().Position = mainCameraEntity.GetComponent<TransformComponent>().Position;
			envEntity.GetComponent<TransformComponent>().Rotation = glm::vec3(0, 0, 0);
			envEntity.GetComponent<TransformComponent>().Scale = glm::vec3(1, 1, 1);
		}

		for (int i = 0; i < Render->normalEntitys.size(); i++)
		{
			if (Render->normalEntitys[i] && !Render->normalEntitys[i].IsGizmo() && Render->normalEntitys[i].HasComponent<StaticMeshComponent>())
			{
				Renderer::DrawRenderer(RHICmdList, Render->normalEntitys[i], true, PSOInit);
			}
		}

		for (int i = 0; i < Render->environmentEntitys.size(); i++)
		{
			Renderer::DrawSky(RHICmdList, Render->environmentEntitys[i]);
		}
		
	}


	void DeferredShadingRenderer::GBufferGeometryPass(Ref<RHICommandList> RHICmdList)
	{
		if (!SceneRenderStates::Get() || !SceneShaderMap::Get())
			return;

		// Set Render Target
		RHICmdList->SetViewport(m_ViewInfo.ViewSize);
		Renderer* Render = Renderer::Get();
		std::vector<Ref<RHITexture2D>> ColorRenderTargets;

		// reset
		RHICmdList->SetTexture(0, nullptr);
		RHICmdList->SetTexture(1, nullptr);
		RHICmdList->SetTexture(2, nullptr);
		RHICmdList->SetTexture(3, nullptr);

		ColorRenderTargets.push_back(SceneRenderTargets::Get()->m_GBufferPosition);
		ColorRenderTargets.push_back(SceneRenderTargets::Get()->m_GBufferAlbedo);
		ColorRenderTargets.push_back(SceneRenderTargets::Get()->m_GBufferNormal);
		ColorRenderTargets.push_back(SceneRenderTargets::Get()->m_GBufferMaterial);
		RHICmdList->RHIClearRenderTarget(ColorRenderTargets, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
			nullptr);
		RHICmdList->SetRenderTarget(ColorRenderTargets, Render->GetSceneRenderTargets()->GetSceneDepthTexture());

		Entity mainCameraEntity = Render->GetEngine()->GetSystem<World>()->GetMainCamera();
		// Update View UniformBuffer
		Renderer::UpdateViewUniformBuffer(RHICmdList, mainCameraEntity);

		GraphicsPipelineStateInitializer PSOInit;
		PSOInit.BoundShaderState.VertexDeclarationRHI = SceneShaderMap::Get()->m_GBufferGeometryDeclaration;
		PSOInit.BoundShaderState.VertexShaderRHI = SceneShaderMap::Get()->m_GBufferGeometryVS;
		PSOInit.BoundShaderState.PixelShaderRHI = SceneShaderMap::Get()->m_GBufferGeometryPS;

		PSOInit.PrimitiveType = EPrimitiveType::PT_TriangleList;
		//PSOInit.BlendState = SceneRenderStates::Get()->;
		//PSOInit.RasterizerState = PSOInitializer.RasterizerState;
		PSOInit.DepthStencilState = SceneRenderStates::Get()->EqualNoWriteDepthStencilState;

		for (int i = 0; i < Render->normalEntitys.size(); i++)
		{
			if (Render->normalEntitys[i] && !Render->normalEntitys[i].IsGizmo() && Render->normalEntitys[i].HasComponent<StaticMeshComponent>())
			{
				Renderer::DrawRenderer(RHICmdList, Render->normalEntitys[i], true, PSOInit);
			}
		}

		PSOInit.BoundShaderState.PixelShaderRHI = SceneShaderMap::Get()->m_GBufferGeometrySkyPS;
		PSOInit.BoundShaderState.VertexShaderRHI = SceneShaderMap::Get()->m_GBufferGeometrySkyVS;
		PSOInit.BoundShaderState.VertexDeclarationRHI = SceneShaderMap::Get()->m_GBufferGeometrySkyDeclaration;
		for (int i = 0; i < Render->environmentEntitys.size(); i++)
		{
			Renderer::DrawSky(RHICmdList, Render->environmentEntitys[i], PSOInit);
		}
	}


	void DeferredShadingRenderer::GBufferLightingPass(Ref<RHICommandList> RHICmdList)
	{
		if (!SceneRenderStates::Get() || !SceneShaderMap::Get())
			return;

		// Set Render Target
		RHICmdList->SetViewport(m_ViewInfo.ViewSize);
		Renderer* Render = Renderer::Get();
	
		RHICmdList->SetRenderTarget(SceneRenderTargets::Get()->GetSceneColorTexture(), SceneRenderTargets::Get()->GetSceneDepthTexture());

		Entity mainCameraEntity = Render->GetEngine()->GetSystem<World>()->GetMainCamera();
		// Update View UniformBuffer
		Renderer::UpdateViewUniformBuffer(RHICmdList, mainCameraEntity);

		GraphicsPipelineStateInitializer PSOInit;
		PSOInit.BoundShaderState.VertexDeclarationRHI = SceneShaderMap::Get()->m_GBufferLightingDeclaration;
		PSOInit.BoundShaderState.VertexShaderRHI = SceneShaderMap::Get()->m_GBufferLightingVS;
		PSOInit.BoundShaderState.PixelShaderRHI = SceneShaderMap::Get()->m_GBufferLightingPS;

		PSOInit.PrimitiveType = EPrimitiveType::PT_TriangleList;
		//PSOInit.BlendState = SceneRenderStates::Get()->;
		//PSOInit.RasterizerState = PSOInitializer.RasterizerState;
		PSOInit.DepthStencilState = SceneRenderStates::Get()->EqualNoWriteDepthStencilState;

		// IBL
		int textureOffset = 3;
		if (Renderer::Get()->m_World->GetMainEnvironment())
		{
			Entity envEntity = Renderer::Get()->m_World->GetMainEnvironment();
			EnvironmentComponent& envComp = envEntity.GetComponent<EnvironmentComponent>();
			RHICmdList->SetTexture(0, envComp.GetEnvDiffuseIrradiance());
			RHICmdList->SetTexture(1, envComp.GetEnvSpecularPrefilter());
			RHICmdList->SetTexture(2, envComp.GetEnvSpecularIntegrateBRDF());
		}

		RHICmdList->SetTexture(textureOffset + 0, SceneRenderTargets::Get()->m_GBufferPosition);
		RHICmdList->SetTexture(textureOffset + 1, SceneRenderTargets::Get()->m_GBufferAlbedo);
		RHICmdList->SetTexture(textureOffset + 2, SceneRenderTargets::Get()->m_GBufferNormal);
		RHICmdList->SetTexture(textureOffset + 3, SceneRenderTargets::Get()->m_GBufferMaterial);
		RHICmdList->SetTexture(textureOffset + 4, SceneRenderTargets::Get()->GetSceneDepthTexture());
		FullScreenUniformParameters fullScreenParameter;
		fullScreenParameter.LocalToWorldMatrix = glm::mat4();
		for (int i = 0; i < Render->lightEntitys.size(); i++)
		{
			// Use Full Pixel Shader to Lighting
			//Renderer::DrawFullScreenQuad(RHICmdList, )
			// Update ObjectBuffer
			SceneUniformBuffers* UniformBuffer = SceneUniformBuffers::Get();
			if (!UniformBuffer || !Renderer::Get())
			{
				return;
			}
			// Update ObjectBuffer
			RHICmdList->SetUniformBuffer(UniformBuffer->FullScreenUniformBuffer->GetSlotIndex(),
				EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
				UniformBuffer->FullScreenUniformBuffer->UniformBuffer());

			UniformBuffer->FullScreenUniformBuffer->UpdateUniformBufferImmediate(fullScreenParameter);
			//
			static std::shared_ptr<RHIVertexShader> FullScreenVertexShader = SceneShaderMap::Get()->m_GBufferLightingVS;
			static std::shared_ptr<RHIPixelShader> FullScreenPixelShader = SceneShaderMap::Get()->m_GBufferLightingPS;
			static std::shared_ptr<RHIVertexDeclaration> FullScreenVertexDeclaration = SceneShaderMap::Get()->m_GBufferLightingDeclaration;
			
			PixelShaderUtils::DrawFullScreenQuad(RHICmdList, FullScreenVertexDeclaration, FullScreenVertexShader, FullScreenPixelShader);
		}

	}


}