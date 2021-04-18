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
#include "World/Entity.h"

namespace Lemon
{
	class DynamicRHI;
	class World;
	
	class LEMON_API Renderer : public ISystem
	{
	public:
		Renderer(Engine* engine);
		~Renderer();


		bool Initialize() override;
		void Tick(float deltaTime) override;

		void OnResize(uint32_t newWidth, uint32_t newHeight);
		
		//====SwapChain=============================//
		const auto& GetSwapChain() const { return m_RHISwapChain; }

		Ref<SceneRenderTargets> GetSceneRenderTargets() const { return m_SceneRenderTargets; }
		Ref<SceneRenderStates> GetSceneRenderStates() const {return m_SceneRenderStates;}
		
		void SetViewport(const Viewport& inViewport) { m_Viewport = inViewport; }
		Viewport GetViewport() const { return m_Viewport;}

		//ConstantBuffer Update
		void UpdateViewUniformBuffer(Entity mainCameraEntity) const;
		
		
	private:
		void InitGeometry();

		void DrawRenderer(Entity entity) const;
		
	private:
		Viewport m_Viewport = { 0, 0, 1920, 1080 };
		Ref<RHISwapChain> m_RHISwapChain;

		Ref<RHICommandList> m_RHICommandList;
		Ref<SceneRenderTargets> m_SceneRenderTargets;
		Ref<SceneUniformBuffers> m_SceneUniformBuffers;
		Ref<SceneRenderStates> m_SceneRenderStates;
		
		World* m_World;
		
		//===============Debug
		Ref<RHIVertexShader> simpleVertexShader;
		Ref<RHIPixelShader> simplePixelShader;
		Ref<RHIVertexBuffer> simpleVertexBuffer;
		Ref<RHIIndexBuffer> simpleIndexBuffer;
		Ref<RHIVertexDeclaration> vertexDeclaration;

	};
}
