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
#include "World/World.h"
#include "World/Components/CameraComponent.h"
#include "World/Components/EnvironmentComponent.h"
#include "World/Components/StaticMeshComponent.h"
#include "World/Components/TransformComponent.h"

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
		// Get the World
		m_World = m_Engine->GetSystem<World>();
		
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

		// Create RenderStates tp initalize some RenderStates
		m_SceneRenderStates = CreateRef<SceneRenderStates>();
		m_SceneRenderStates->Allocate();
		m_SceneRenderStates->SetGlobalSampler(m_RHICommandList);
		
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
		return;

#if 0
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
#endif
		
	}

	void Renderer::DrawRenderer(Entity entity) const
	{
		TransformComponent& transformComp = entity.GetComponent<TransformComponent>();
		StaticMeshComponent& staticMeshComp = entity.GetComponent<StaticMeshComponent>();

		// Update ObjectBuffer
		m_RHICommandList->SetUniformBuffer(1,
            EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
            m_SceneUniformBuffers->ObjectUniformBuffer->UniformBuffer());
		ObjectUniformParameters parameters;
		parameters.LocalToWorldMatrix = transformComp.GetTransform();
		parameters.Color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		m_SceneUniformBuffers->ObjectUniformBuffer->UpdateUniformBufferImmediate(parameters);

		// Set PSO
		GraphicsPipelineStateInitializer PSOInit;
		PSOInit.BoundShaderState.PixelShaderRHI = staticMeshComp.GetRenderMesh()->GetPixelShader();
		PSOInit.BoundShaderState.VertexShaderRHI = staticMeshComp.GetRenderMesh()->GetVertexShader();
		PSOInit.BoundShaderState.VertexDeclarationRHI = staticMeshComp.GetRenderMesh()->GetVertexDeclaration();
		PSOInit.PrimitiveType = staticMeshComp.GetRenderMesh()->GetPrimitiveType();//EPrimitiveType::PT_TriangleList;
		PSOInit.BlendState = staticMeshComp.GetRenderMesh()->GetBlendState();
		PSOInit.RasterizerState = staticMeshComp.GetRenderMesh()->GetRasterizerState();
		PSOInit.DepthStencilState = staticMeshComp.GetRenderMesh()->GetDepthStencilState();
		
		m_RHICommandList->SetGraphicsPipelineState(PSOInit);

		// Set VertexBuffer and IndexBuffer
		m_RHICommandList->SetIndexBuffer(staticMeshComp.GetRenderMesh()->GetIndexBuffer());
		m_RHICommandList->SetVertexBuffer(0, staticMeshComp.GetRenderMesh()->GetVertexBuffer());
		// Set Textures
		for(int i = 0;i < staticMeshComp.GetRenderMesh()->GetTextures().size(); i++)
		{
			m_RHICommandList->SetTexture(staticMeshComp.GetRenderMesh()->GetTextureStartSlot() + i, staticMeshComp.GetRenderMesh()->GetTextures()[i]);
		}
		
		// Draw
		m_RHICommandList->DrawIndexPrimitive(0, 0, staticMeshComp.GetRenderMesh()->GetIndexCount() / 3);
	}

	void Renderer::OnResize(uint32_t newWidth, uint32_t newHeight)
	{
		m_Viewport.Width = (float)newWidth;
		m_Viewport.Height = (float)newHeight;
		
		m_SceneRenderTargets->OnResize(newWidth, newHeight);

		if(m_World->GetMainCamera())
		{
			m_World->GetMainCamera().GetComponent<CameraComponent>().SetViewportSize(newWidth, newHeight);
		}

	}
	void Renderer::Tick(float deltaTime)
	{
		m_RHICommandList->SetViewport(m_Viewport);
		m_RHICommandList->SetRenderTarget(GetSceneRenderTargets()->GetSceneColorTexture(), GetSceneRenderTargets()->GetSceneDepthTexture());
		m_RHICommandList->RHIClearRenderTarget(GetSceneRenderTargets()->GetSceneColorTexture(),glm::vec4(0.1f, 0.4f, 0.7f, 1.0f),
			GetSceneRenderTargets()->GetSceneDepthTexture());

		UpdateViewUniformBuffer();

		std::vector<Entity> entitys = m_World->GetAllEntities();

		std::vector<Entity> environments; // Environment Entity need draw Last
		for(int i = 0;i < entitys.size(); i++)
		{
			if(entitys[i] && !entitys[i].IsGizmo() && entitys[i].HasComponent<StaticMeshComponent>())
			{
				if(entitys[i].HasComponent<EnvironmentComponent>())
					environments.emplace_back(entitys[i]);
				else
					DrawRenderer(entitys[i]);
			}
		}

		//Draw Last
		for(int i = 0;i < environments.size(); i++)
		{
			if(environments[i] && !environments[i].IsGizmo() && environments[i].HasComponent<EnvironmentComponent>())
			{
				DrawRenderer(environments[i]);
			}
		}

		//Draw Debug Gizmo
		std::vector<Entity> GizmoDebugEntitys = m_World->GetGizmoDebugEntitys();
		for (int i = 0; i < GizmoDebugEntitys.size(); i++)
		{
			if (GizmoDebugEntitys[i] && GizmoDebugEntitys[i].IsGizmo() && GizmoDebugEntitys[i].HasComponent<StaticMeshComponent>())
			{
				DrawRenderer(GizmoDebugEntitys[i]);
			}
		}

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
		Entity mainCameraEntity = m_World->GetMainCamera();

		TransformComponent& transformComp = mainCameraEntity.GetComponent<TransformComponent>();
		CameraComponent& mainCameraComp = mainCameraEntity.GetComponent<CameraComponent>();
		glm::mat4 cameraTransform = transformComp.GetTransform();
		parameters.ViewMatrix = mainCameraComp.GetViewMatrix(); //glm::inverse(transformComp.GetTransform());
		parameters.ProjectionMatrix = mainCameraComp.GetProjectionMatrix();
		parameters.ViewProjectionMatrix = parameters.ProjectionMatrix * parameters.ViewMatrix;

		//glm::vec4 debugPoint1 = parameters.ViewMatrix * glm::vec4(-0.5f, 0.5f, 1.0f, 1.0f);
		//glm::vec4 debugPoint = parameters.ProjectionMatrix * debugPoint1;
		
		/*
		 consider EnvironmentEntity to snap transform to camera transform
		*/
		for(int i = 0; i < m_World->GetAllEnvironmentEntitys().size(); i++)
		{
			Entity envEntity = m_World->GetAllEnvironmentEntitys()[i];
			envEntity.GetComponent<TransformComponent>().Position = transformComp.Position;
			envEntity.GetComponent<TransformComponent>().Rotation = glm::vec3(0, 0, 0);
			envEntity.GetComponent<TransformComponent>().Scale = glm::vec3(1, 1, 1);
		}
		
		parameters.TestColor = glm::vec3(preRColorValue,accTime,accTime);
		m_SceneUniformBuffers->ViewUniformBuffer->UpdateUniformBufferImmediate(parameters);
	}
	
}
