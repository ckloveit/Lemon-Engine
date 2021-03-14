#include "LemonPCH.h"
#include "Renderer.h"
#include "Core/Engine.h"
#include "RHI/RHI.h"
#include "RHI/DynamicRHI.h"
#include "Log/Log.h"
#include "RHI/RHISwapChain.h"
#include "Containers/DynamicRHIResourceArray.h"
#include <glm/gtc/random.hpp>

#include "Core/Timer.h"

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

		m_SceneUniformBuffers = CreateRef<SceneUniformBuffers>();
		
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
		m_Cube = CreateScope<Cube>();
		m_Cube->CreateShader<SF_Vertex>("Assets/Shaders/SimpleStandardVertex.hlsl", "MainVS");
		m_Cube->CreateShader<SF_Pixel>("Assets/Shaders/SimpleStandardPixel.hlsl", "MainPS");
		m_Cube->CreateRHIBuffers();

		return;
		
		RHIShaderCreateInfo shaderCreateInfo;
		simpleVertexShader = RHICreateVertexShader("Assets/Shaders/SimpleColorVertex.hlsl", "MainVS", shaderCreateInfo);
		simplePixelShader = RHICreatePixelShader("Assets/Shaders/SimpleColorPixel.hlsl", "MainPS", shaderCreateInfo);

		RHIResourceCreateInfo vertexCreateInfo;
		TResourceArray<SimpleVertex> verts;
		verts.PushBack({ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec4(1.0f, 0.0f, 0.0f,1.0f) });
		verts.PushBack({ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 1.0f, 0.0f,1.0f) });
		verts.PushBack({ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 1.0f,1.0f) });
		verts.PushBack({ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec4(1.0f, 1.0f, 1.0f,1.0f) });

		vertexCreateInfo.ResourceArray = &verts;
		simpleVertexBuffer = RHICreateVertexBuffer(sizeof(SimpleVertex) * 4, BUF_Static, vertexCreateInfo);

		TResourceArray<uint32_t> indices;
		indices.PushBack(0);
		indices.PushBack(1);
		indices.PushBack(2);
		  
		indices.PushBack(0);
		indices.PushBack(2);
		indices.PushBack(3);

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

	}

	void Renderer::DrawMeshRenderer(Mesh* mesh) const
	{
		GraphicsPipelineStateInitializer PSOInit;
		PSOInit.BoundShaderState.PixelShaderRHI = mesh->GetPixelShader();
		PSOInit.BoundShaderState.VertexShaderRHI = mesh->GetVertexShader();
		PSOInit.BoundShaderState.VertexDeclarationRHI = mesh->GetVertexDeclaration();
		PSOInit.PrimitiveType = EPrimitiveType::PT_TriangleList;
		m_RHICommandList->SetGraphicsPipelineState(PSOInit);

		m_RHICommandList->SetIndexBuffer(mesh->GetIndexBuffer());
		m_RHICommandList->SetVertexBuffer(0, mesh->GetVertexBuffer());

		m_RHICommandList->DrawIndexPrimitive(0, 0, mesh->GetIndexCount() / 3);
	}

	void Renderer::OnResize(uint32_t newWidth, uint32_t newHeight)
	{
		m_Viewport.Width = newWidth;
		m_Viewport.Height = newHeight;
		
		m_SceneRenderTargets->OnResize(newWidth, newHeight);
		
	}
	void Renderer::Tick(float deltaTime)
	{
		m_RHICommandList->SetViewport(m_Viewport);
		m_RHICommandList->SetRenderTarget(GetSceneRenderTargets()->GetSceneColorTexture());
		m_RHICommandList->RHIClearRenderTarget(GetSceneRenderTargets()->GetSceneColorTexture(), glm::vec4(0.1f, 0.4f, 0.7f, 1.0f));

		UpdateViewUniformBuffer();
		
		DrawMeshRenderer(m_Cube.get());
		
		/*
		GraphicsPipelineStateInitializer PSOInit;
		PSOInit.BoundShaderState.PixelShaderRHI = simplePixelShader;
		PSOInit.BoundShaderState.VertexShaderRHI = simpleVertexShader;
		PSOInit.BoundShaderState.VertexDeclarationRHI = vertexDeclaration;
		PSOInit.PrimitiveType = EPrimitiveType::PT_TriangleList;
		m_RHICommandList->SetGraphicsPipelineState(PSOInit);

		m_RHICommandList->SetIndexBuffer(simpleIndexBuffer);
		m_RHICommandList->SetVertexBuffer(0, simpleVertexBuffer);

		m_RHICommandList->DrawIndexPrimitive(0, 0, 2);

		//m_RHISwapChain->Present();
		//m_RHICommandList->Flush();
		*/
	}

	void Renderer::UpdateViewUniformBuffer() const
	{
		m_RHICommandList->SetUniformBuffer(0,
			EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
			m_SceneUniformBuffers->ViewUniformBuffer->UniformBuffer());
		ViewUniformParameters parameters;
		static float accTime = 0;
		static float preRColorValue = 1.0f;
		static float preGColorValue = 0.0f;
		static float preBColorValue = 0.0f;
		accTime += m_Engine->GetSystem<Timer>()->GetDeltaTimeSec();
		if(accTime > 1.0f)
		{
			accTime = 0;
			preRColorValue = glm::linearRand(0, 1);
		}
		parameters.WorldToProjection = glm::mat4();
		parameters.TestColor = glm::vec3(preRColorValue,accTime,accTime);
		
		m_SceneUniformBuffers->ViewUniformBuffer->UpdateUniformBufferImmediate(parameters);
		
	}
}
