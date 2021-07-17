#include "LemonPCH.h"
#include "ForwardShadingRenderer.h"
#include "Renderer.h"
#include "Core/Engine.h"
#include "World/World.h"

#include "World/Components/TransformComponent.h"
#include "World/Components/StaticMeshComponent.h"


namespace Lemon
{
	void ForwardShadingRenderer::Render(Ref<RHICommandList> RHICmdList)
	{
		// Set Render Target
		RHICmdList->SetViewport(m_ViewInfo.ViewSize);
		Renderer* Render = Renderer::Get();
		if (Render->GetEngine()->bShowImGuiEditor)
		{
			RHICmdList->SetRenderTarget(Render->GetSceneRenderTargets()->GetSceneColorTexture(), Render->GetSceneRenderTargets()->GetSceneDepthTexture());
			RHICmdList->RHIClearRenderTarget(Render->GetSceneRenderTargets()->GetSceneColorTexture(), glm::vec4(0.1f, 0.4f, 0.7f, 1.0f),
				Render->GetSceneRenderTargets()->GetSceneDepthTexture());
		}
		else
		{
			RHICmdList->SetRenderTarget(Render->GetSwapChain());
		}

		Entity mainCameraEntity = Render->m_World->GetMainCamera();
		// Update View UniformBuffer
		Renderer::UpdateViewUniformBuffer(RHICmdList, mainCameraEntity);

		// Update Light UniformBuffer
		Renderer::UpdateLightUniformBuffer(RHICmdList, Render->lightEntitys);

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
				Renderer::DrawRenderer(RHICmdList, Render->normalEntitys[i], false);
			}
		}

		for (int i = 0; i < Render->environmentEntitys.size(); i++)
		{
			Renderer::DrawSky(RHICmdList, Render->environmentEntitys[i]);
		}
		//Draw Debug Gizmo
		for (int i = 0; i < Render->gizmoDebugEntitys.size(); i++)
		{
			if (Render->gizmoDebugEntitys[i] && Render->gizmoDebugEntitys[i].IsGizmo() && Render->gizmoDebugEntitys[i].HasComponent<StaticMeshComponent>())
			{
				Renderer::DrawRenderer(RHICmdList, Render->gizmoDebugEntitys[i], false);
			}
		}

		// Debug 
		if (Render->environmentEntitys[0].HasComponent<EnvironmentComponent>())
		{
			EnvironmentComponent& envComp = Render->environmentEntitys[0].GetComponent<EnvironmentComponent>();
			if (envComp.bDebugShowIBLType == 3)
			{
				FullScreenUniformParameters fullScreenParameter;
				fullScreenParameter.LocalToWorldMatrix = glm::mat4();
				std::vector<std::shared_ptr<RHITexture>> textures;
				textures.emplace_back(envComp.GetEnvSpecularIntegrateBRDF());
				Renderer::DrawFullScreenQuad(RHICmdList, fullScreenParameter, textures);
			}
		}

	}
}