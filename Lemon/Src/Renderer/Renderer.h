#pragma once
#include "Core/Core.h"
#include "Core/ISystem.h"

#include <glm/glm.hpp>


#include "SceneRenderStates.h"
#include "RenderCore/Viewport.h"
#include "RHI/RHI.h"

#include "SceneRenderTargets.h"
#include "SceneUniformBuffers.h"
#include "RHI/RHICommandList.h"
#include "RenderCore/Mesh.h"
#include "RenderCore/Geometry/Cube.h"
#include "RenderCore/Geometry/Quad.h"
#include "ForwardShadingRenderer.h"
#include "DeferredShadingRenderer.h"
#include "SceneShaderMap.h"


#include "World/Entity.h"
#include "World/Components/EnvironmentComponent.h"


namespace Lemon
{
	enum EShadingPath
	{
		Forward,
		Deferred
	};

	class DynamicRHI;
	class World;
	class GlobalRenderResources;

	class LEMON_API Renderer : public ISystem
	{
		friend class ForwardShadingRenderer;
		friend class DeferredShadingRenderer;

	public:
		Renderer(Engine* engine);
		~Renderer();

		bool Initialize() override;
		void Tick(float deltaTime) override;

		void PreRender(float deltaTime);

		void OnResize(uint32_t newWidth, uint32_t newHeight);
		
		Engine* GetEngine() { return m_Engine; }
		//====SwapChain=============================//
		const auto& GetSwapChain() const { return m_RHISwapChain; }

		Ref<SceneRenderTargets> GetSceneRenderTargets() const { return m_SceneRenderTargets; }
		Ref<SceneRenderStates> GetSceneRenderStates() const {return m_SceneRenderStates;}
		
		void SetViewport(const Viewport& inViewport) { m_Viewport = inViewport; }
		Viewport GetViewport() const { return m_Viewport;}

		EShadingPath GetShadingPath() { return m_ShadingPath; }

	public:
		static Renderer* Get() { return s_Instance; }
		static void DrawRenderer(Ref<RHICommandList> RHICmdList, Entity entity, 
			bool bPSOInit, GraphicsPipelineStateInitializer PSOInitializer = {}, int textureOffset = 3);
		static void DrawSky(Ref<RHICommandList> RHICmdList, Entity entity, GraphicsPipelineStateInitializer PSOInitializer = {});
		//ConstantBuffer Update
		static void UpdateViewUniformBuffer(Ref<RHICommandList> RHICmdList, Entity mainCameraEntity);
		static void UpdateLightUniformBuffer(Ref<RHICommandList> RHICmdList, const std::vector<Entity>& lightEntitys);
		static void DrawFullScreenQuad(Ref<RHICommandList> RHICmdList, FullScreenUniformParameters fullScreenParameter,
			std::vector<std::shared_ptr<RHITexture>> Textures = std::vector<std::shared_ptr<RHITexture>>());



	private:
		void InitGeometry();


		
		void PreComputeIBL(std::vector<Entity>& environment);

		void EnvEquirectangularToCubeMap(EnvironmentComponent& envComp);
		// ====IBL=======

		void ComputePreDiffuseIrradiance(FullScreenUniformParameters fullScreenParameter);
		void ComputePreFilterSepcualr(FullScreenUniformParameters fullScreenParameter, CustomDataFloat4UniformParameters customDataFloat4Parameter);
		void ComputePreIntegrateBRDF(FullScreenUniformParameters fullScreenParameter);

		//===============
	private:
		Viewport m_Viewport = { 0, 0, 1920, 1080 };
		Ref<RHISwapChain> m_RHISwapChain;

		Ref<RHICommandList> m_RHICommandList;
		Ref<SceneRenderTargets> m_SceneRenderTargets;
		Ref<SceneUniformBuffers> m_SceneUniformBuffers;
		Ref<SceneRenderStates> m_SceneRenderStates;
		Ref<SceneShaderMap> m_SceneShaderMap;

		// use for FullScreen
		Ref<Quad> m_FullScreenQuad;
		
		World* m_World;
		
		//===============Debug
		Ref<RHIVertexShader> simpleVertexShader;
		Ref<RHIPixelShader> simplePixelShader;
		Ref<RHIVertexBuffer> simpleVertexBuffer;
		Ref<RHIIndexBuffer> simpleIndexBuffer;
		Ref<RHIVertexDeclaration> vertexDeclaration;

		// ---------classfiy entitys
		std::vector<Entity> gizmoDebugEntitys;
		std::vector<Entity> environmentEntitys;
		std::vector<Entity> normalEntitys;
		std::vector<Entity> lightEntitys;

		// Render Shading Path
		Ref<SceneRenderer> m_ShadingRenderer = nullptr;
		EShadingPath m_ShadingPath = EShadingPath::Deferred;//EShadingPath::Forward ;//

		// Static Instance
		static Renderer* s_Instance;
	};
}
