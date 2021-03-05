#include "LemonPCH.h"
#include "Renderer.h"
#include "Core/Engine.h"
#include "RHI/RHI.h"
#include "RHI/DynamicRHI.h"
#include "Log/Log.h"
#include "RHI/RHISwapChain.h"
#include "Containers/DynamicRHIResourceArray.h"


namespace Lemon
{
	Renderer::Renderer(Engine* engine)
		:ISystem(engine)
	{

	}
	Renderer::~Renderer()
	{
		RHIExit();
	}

	bool Renderer::Initialize()
	{
		// Resolution, viewport and swapchain default to whatever the window size is
		const WindowData& windowData = m_Engine->GetWindowData();

		// Set viewport
		m_Viewport.Width = windowData.Width;
		m_Viewport.Height = windowData.Height;

		// Init RHI
		RHIInit();

		m_RHISwapChain = RHICreateSwapChain(windowData.Handle,
			static_cast<uint32_t>(m_Viewport.Width),
			static_cast<uint32_t>(m_Viewport.Height),
			RHI_PF_R8G8B8A8_Unorm);

		if (!m_RHISwapChain)
		{
			LEMON_CORE_ERROR("Failed to create swap chain");
			return false;
		}

		// Create RHICommandList
		m_RHICommandList = RHICreateCommandList(this);
		// Create RenderTargetTextures
		m_SceneRenderTargets = CreateRef<SceneRenderTargets>(m_Viewport.Width, m_Viewport.Height);
		m_SceneRenderTargets->Allocate(m_RHICommandList);
		
		// Init Others
		InitGeometry();
		return true;
	}
	struct SimpleVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

	};
	void Renderer::InitGeometry()
	{
		return;
		RHIShaderCreateInfo shaderCreateInfo;
		simpleVertexShader = RHICreateVertexShader("Assets/Shaders/SimpleColorVertex.hlsl", "MainVS", shaderCreateInfo);
		simplePixelShader = RHICreatePixelShader("Assets/Shaders/SimpleColorPixel.hlsl", "MainPS", shaderCreateInfo);

		RHIResourceCreateInfo vertexCreateInfo;
		TResourceArray<SimpleVertex> verts;
		verts.PushBack({ glm::vec3(0.0f, 0.5f, 0.5f), glm::vec4(1.0f, 0.0f, 0.0f,1.0f) });
		verts.PushBack({ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 1.0f, 0.0f,1.0f) });
		verts.PushBack({ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 1.0f,1.0f) });
		//verts.PushBack({ glm::vec3( 0.5f, -0.5f, 0.5f), glm::vec4(1.0f, 1.0f, 1.0f,1.0f) });

		vertexCreateInfo.ResourceArray = &verts;
		simpleVertexBuffer = RHICreateVertexBuffer(sizeof(SimpleVertex) * 4, BUF_Static, vertexCreateInfo);

		TResourceArray<uint32_t> indices;
		indices.PushBack(0);
		indices.PushBack(1);
		indices.PushBack(2);

		//indices.PushBack(2);
		//indices.PushBack(3);
		//indices.PushBack(0);
		RHIResourceCreateInfo indicesCreateInfo;
		indicesCreateInfo.ResourceArray = &indices;
		simpleIndexBuffer = RHICreateIndexBuffer(sizeof(uint32_t) * 6, BUF_Static, indicesCreateInfo);

		VertexDeclarationElementList vertexElements;
		uint32_t stride = sizeof(SimpleVertex);
		vertexElements.push_back(RHIVertexElement(0, STRUCT_OFFSET(SimpleVertex, Position), VET_Float3, 0, stride));
		vertexElements.push_back(RHIVertexElement(0, STRUCT_OFFSET(SimpleVertex, Color), VET_Float4, 1, stride));
		vertexDeclaration = RHICreateVertexDeclaration(simpleVertexShader, vertexElements);

		GraphicsPipelineStateInitializer PSOInit;
		PSOInit.BoundShaderState.PixelShaderRHI = simplePixelShader;
		PSOInit.BoundShaderState.VertexShaderRHI = simpleVertexShader;
		PSOInit.BoundShaderState.VertexDeclarationRHI = vertexDeclaration;
		PSOInit.PrimitiveType = EPrimitiveType::PT_TriangleList;
		m_RHICommandList->SetGraphicsPipelineState(PSOInit);

		m_RHICommandList->SetIndexBuffer(simpleIndexBuffer);
		m_RHICommandList->SetVertexBuffer(0, simpleVertexBuffer);

	}

	void Renderer::Tick(float deltaTime)
	{
		m_RHICommandList->SetViewport(m_Viewport);
		//m_RHICommandList->SetRenderTarget(GetSceneRenderTargets()->GetSceneColorTexture());
		//m_RHICommandList->DebugRenderer(GetSceneRenderTargets()->GetSceneColorTexture());

		m_RHICommandList->SetRenderTarget(GetSceneRenderTargets()->GetSceneColorTexture());
		m_RHICommandList->DebugRenderer(nullptr);
		
		return;
		/*
		if (m_RHISwapChain)
		{
			RHIClearRenderTarget(m_RHISwapChain, { 1.0f, 1.0f, 0.0f, 1.0f });
		}*/
		m_RHICommandList->SetRenderTarget(GetSceneRenderTargets()->GetSceneColorTexture());
		m_RHICommandList->SetViewport(m_Viewport);
		//m_RHICommandList->RHIClearRenderTarget(GetSceneRenderTargets()->GetSceneColorTexture(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		//m_RHICommandList->SetRenderTarget(m_RHISwapChain);
		m_RHICommandList->RHIClearRenderTarget(GetSceneRenderTargets()->GetSceneColorTexture(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		GraphicsPipelineStateInitializer PSOInit;
		PSOInit.BoundShaderState.PixelShaderRHI = simplePixelShader;
		PSOInit.BoundShaderState.VertexShaderRHI = simpleVertexShader;
		PSOInit.BoundShaderState.VertexDeclarationRHI = vertexDeclaration;
		PSOInit.PrimitiveType = EPrimitiveType::PT_TriangleList;
		//m_RHICommandList->SetGraphicsPipelineState(PSOInit);

		//m_RHICommandList->SetIndexBuffer(simpleIndexBuffer);
		//m_RHICommandList->SetVertexBuffer(0, simpleVertexBuffer);

		m_RHICommandList->DrawIndexPrimitive(0, 0, 1);

		//m_RHISwapChain->Present();
		//m_RHICommandList->Flush();
	}

}