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
#include "World/Entity.h"
#include "World/Components/EnvironmentComponent.h"

namespace Lemon
{
	class DynamicRHI;
	class World;
	class GlobalRenderResources;

	class LEMON_API Renderer : public ISystem
	{
	public:
		Renderer(Engine* engine);
		~Renderer();

		bool Initialize() override;
		void Tick(float deltaTime) override;


		void PreRender(float deltaTime);
		void Render(float deltaTime);

		void OnResize(uint32_t newWidth, uint32_t newHeight);
		
		//====SwapChain=============================//
		const auto& GetSwapChain() const { return m_RHISwapChain; }

		Ref<SceneRenderTargets> GetSceneRenderTargets() const { return m_SceneRenderTargets; }
		Ref<SceneRenderStates> GetSceneRenderStates() const {return m_SceneRenderStates;}
		
		void SetViewport(const Viewport& inViewport) { m_Viewport = inViewport; }
		Viewport GetViewport() const { return m_Viewport;}

		//ConstantBuffer Update
		void UpdateViewUniformBuffer(Entity mainCameraEntity) const;

		void UpdateLightUniformBuffer(const std::vector<Entity>& lightEntitys) const;
		
	private:
		void InitGeometry();

		void DrawRenderer(Entity entity) const;
		void DrawSky(Entity entity) const;


		void DrawFullScreenQuad(FullScreenUniformParameters fullScreenParameter, std::vector<std::shared_ptr<RHITexture>> Textures = std::vector<std::shared_ptr<RHITexture>>());

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

	};
}
