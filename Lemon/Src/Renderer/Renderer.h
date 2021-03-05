#pragma once
#include "Core/Core.h"
#include "Core/ISystem.h"

#include <glm/glm.hpp>

#include "RenderCore/Viewport.h"
#include "RHI/RHI.h"

#include "SceneRenderTargets.h"
#include "RHI/RHICommandList.h"

namespace Lemon
{
	class DynamicRHI;

	class LEMON_API Renderer : public ISystem
	{
	public:
		Renderer(Engine* engine);
		~Renderer();


		bool Initialize() override;
		void Tick(float deltaTime) override;


		//====SwapChain=============================//
		const auto& GetSwapChain() const { return m_RHISwapChain; }

		Ref<SceneRenderTargets> GetSceneRenderTargets() const { return m_SceneRenderTargets; }
	private:
		void InitGeometry();
	private:
		Viewport m_Viewport = { 0, 0, 1920, 1080 };
		Ref<RHISwapChain> m_RHISwapChain;

		Ref<RHICommandList> m_RHICommandList;
		Ref<SceneRenderTargets> m_SceneRenderTargets;


		//===============Debug
		Ref<RHIVertexShader> simpleVertexShader;
		Ref<RHIPixelShader> simplePixelShader;
		Ref<RHIVertexBuffer> simpleVertexBuffer;
		Ref<RHIIndexBuffer> simpleIndexBuffer;
		Ref<RHIVertexDeclaration> vertexDeclaration;

	};
}
