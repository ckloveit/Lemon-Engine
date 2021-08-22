#include "LemonPCH.h"
#include "Renderer.h"
#include "Core/Engine.h"
#include "RHI/RHI.h"
#include "RHI/DynamicRHI.h"
#include "Log/Log.h"
#include "RHI/RHISwapChain.h"
#include "RenderCore/Containers/DynamicRHIResourceArray.h"
#include <glm/gtc/random.hpp>
#include "Core/Timer.h"
#include "World/World.h"
#include "World/Components/CameraComponent.h"
#include "World/Components/DirectionalLightComponent.h"
#include "World/Components/EnvironmentComponent.h"
#include "World/Components/StaticMeshComponent.h"
#include "World/Components/TransformComponent.h"
#include "RenderCore/GlobalRenderResources.h"
#include "RenderCore/PixelShaderUtils.h"
//
#include "Core/Engine.h"
#include "Core/SystemManager.h"
#include "Input/InputSystem.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
namespace Lemon
{
	Renderer* Renderer::s_Instance = nullptr;

	Renderer::Renderer(Engine* engine)
		:ISystem(engine)
	{
		s_Instance = this;

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
		m_SceneRenderTargets->Allocate(m_RHICommandList, this);

		// ReSize Renderer
		this->OnResize(m_Viewport.Width, m_Viewport.Height);

		// Create RenderStates tp initalize some RenderStates
		m_SceneRenderStates = CreateRef<SceneRenderStates>();
		m_SceneRenderStates->Allocate();
		m_SceneRenderStates->SetGlobalSampler(m_RHICommandList);
		
		m_SceneUniformBuffers = CreateRef<SceneUniformBuffers>();
		m_SceneUniformBuffers->Allocate();

		// Init Global Render Resources
		GlobalRenderResources::Init();

		m_SceneShaderMap = CreateRef<SceneShaderMap>();
		m_SceneShaderMap->Allocate();

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
		m_FullScreenQuad = CreateRef<Quad>();
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

	void Renderer::DrawRenderer(Ref<RHICommandList> RHICmdList, Entity entity, 
		bool bPSOInit, GraphicsPipelineStateInitializer PSOInitializer, int textureOffset)
	{
		TransformComponent& transformComp = entity.GetComponent<TransformComponent>();
		StaticMeshComponent& staticMeshComp = entity.GetComponent<StaticMeshComponent>();
		if (!staticMeshComp.IsVisiable())
			return;
		SceneUniformBuffers* UniformBuffer = SceneUniformBuffers::Get();
		if (!UniformBuffer || !Renderer::Get())
		{
			return;
		}
		// Update ObjectBuffer
		RHICmdList->SetUniformBuffer(UniformBuffer->ObjectUniformBuffer->GetSlotIndex(),
            EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
			UniformBuffer->ObjectUniformBuffer->UniformBuffer());

		ObjectUniformParameters parameters;
		parameters.LocalToWorldMatrix = transformComp.GetTransform();
		parameters.WorldToWorldMatrix = glm::inverse(transformComp.GetTransform());
		parameters.WorldToWorldTransposeMatrix = glm::transpose(parameters.WorldToWorldMatrix);

		parameters.Color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

		// Set PSO
		GraphicsPipelineStateInitializer PSOInit;
		if (bPSOInit)
		{
			PSOInit.BoundShaderState.PixelShaderRHI = PSOInitializer.BoundShaderState.PixelShaderRHI;
			PSOInit.BoundShaderState.VertexShaderRHI = PSOInitializer.BoundShaderState.VertexShaderRHI;
			PSOInit.BoundShaderState.VertexDeclarationRHI = PSOInitializer.BoundShaderState.VertexDeclarationRHI;
			PSOInit.PrimitiveType = PSOInitializer.PrimitiveType;//EPrimitiveType::PT_TriangleList;
			PSOInit.BlendState = PSOInitializer.BlendState;
			PSOInit.RasterizerState = PSOInitializer.RasterizerState;
			PSOInit.DepthStencilState = PSOInitializer.DepthStencilState;
		}
		else
		{
			PSOInit.BoundShaderState.PixelShaderRHI = staticMeshComp.GetRenderMesh()->GetPixelShader();
			PSOInit.BoundShaderState.VertexShaderRHI = staticMeshComp.GetRenderMesh()->GetVertexShader();
			PSOInit.BoundShaderState.VertexDeclarationRHI = staticMeshComp.GetRenderMesh()->GetVertexDeclaration();
			if (staticMeshComp.GetRenderMesh()->GetMaterial())
			{
				Ref<Material> mat = staticMeshComp.GetRenderMesh()->GetMaterial();
				PSOInit.PrimitiveType = staticMeshComp.GetRenderMesh()->GetMaterial()->GetPrimitiveType();//EPrimitiveType::PT_TriangleList;
				PSOInit.BlendState = staticMeshComp.GetRenderMesh()->GetMaterial()->GetBlendState();
				PSOInit.RasterizerState = staticMeshComp.GetRenderMesh()->GetMaterial()->GetRasterizerState();
				PSOInit.DepthStencilState = staticMeshComp.GetRenderMesh()->GetMaterial()->GetDepthStencilState();
				//PBR Properties
				parameters.Albedo = glm::vec4(mat->Albedo, 1.0f);
				parameters.PBRParameters = glm::vec4(mat->Metallic, mat->Roughness, mat->AO, 1.0f);
			}
			else
			{
				parameters.Albedo = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);// default 
				parameters.PBRParameters = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);// default 
				PSOInit.PrimitiveType = EPrimitiveType::PT_TriangleList;
			}
		}
		

		UniformBuffer->ObjectUniformBuffer->UpdateUniformBufferImmediate(parameters);
		
		RHICmdList->SetGraphicsPipelineState(PSOInit);

		// Set VertexBuffer and IndexBuffer
		RHICmdList->SetIndexBuffer(staticMeshComp.GetRenderMesh()->GetIndexBuffer());
		RHICmdList->SetVertexBuffer(0, staticMeshComp.GetRenderMesh()->GetVertexBuffer());
	
		// IBL
		if (Renderer::Get()->m_World->GetMainEnvironment())
		{
			Entity envEntity = Renderer::Get()->m_World->GetMainEnvironment();
			EnvironmentComponent& envComp = envEntity.GetComponent<EnvironmentComponent>();
			RHICmdList->SetTexture(0, envComp.GetEnvDiffuseIrradiance());
			RHICmdList->SetTexture(1, envComp.GetEnvSpecularPrefilter());
			RHICmdList->SetTexture(2, envComp.GetEnvSpecularIntegrateBRDF());
		}
		// Set PBR Textures
		if (staticMeshComp.GetRenderMesh()->GetMaterial())
		{
			for (int i = 0; i < staticMeshComp.GetRenderMesh()->GetMaterial()->GetTextures().size(); i++)
			{
				RHICmdList->SetTexture(textureOffset + staticMeshComp.GetRenderMesh()->GetMaterial()->GetTextureStartSlot() + i, staticMeshComp.GetRenderMesh()->GetMaterial()->GetTextures()[i]);
			}
		}

		// Draw
		RHICmdList->DrawIndexPrimitive(0, 0, staticMeshComp.GetRenderMesh()->GetIndexCount() / 3);
	}

	void Renderer::DrawSky(Ref<RHICommandList> RHICmdList, Entity entity, GraphicsPipelineStateInitializer PSOInitializer)
	{
		TransformComponent& transformComp = entity.GetComponent<TransformComponent>();
		StaticMeshComponent& staticMeshComp = entity.GetComponent<StaticMeshComponent>();
		if (!staticMeshComp.IsVisiable())
			return;
		SceneUniformBuffers* UniformBuffer = SceneUniformBuffers::Get();
		if (!UniformBuffer || !Renderer::Get())
		{
			return;
		}
		// Update ObjectBuffer
		RHICmdList->SetUniformBuffer(UniformBuffer->ObjectUniformBuffer->GetSlotIndex(),
			EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
			UniformBuffer->ObjectUniformBuffer->UniformBuffer());

		ObjectUniformParameters parameters;
		parameters.LocalToWorldMatrix = transformComp.GetTransform();
		parameters.WorldToWorldMatrix = glm::inverse(transformComp.GetTransform());
		parameters.WorldToWorldTransposeMatrix = glm::transpose(parameters.WorldToWorldMatrix);

		parameters.Color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

		// Set PSO
		GraphicsPipelineStateInitializer PSOInit;

		PSOInit.BoundShaderState.PixelShaderRHI = staticMeshComp.GetRenderMesh()->GetPixelShader();
		PSOInit.BoundShaderState.VertexShaderRHI = staticMeshComp.GetRenderMesh()->GetVertexShader();
		PSOInit.BoundShaderState.VertexDeclarationRHI = staticMeshComp.GetRenderMesh()->GetVertexDeclaration();
		
		if (staticMeshComp.GetRenderMesh()->GetMaterial())
		{
			Ref<Material> mat = staticMeshComp.GetRenderMesh()->GetMaterial();
			PSOInit.PrimitiveType = staticMeshComp.GetRenderMesh()->GetMaterial()->GetPrimitiveType();//EPrimitiveType::PT_TriangleList;
			PSOInit.BlendState = staticMeshComp.GetRenderMesh()->GetMaterial()->GetBlendState();
			PSOInit.RasterizerState = staticMeshComp.GetRenderMesh()->GetMaterial()->GetRasterizerState();
			PSOInit.DepthStencilState = staticMeshComp.GetRenderMesh()->GetMaterial()->GetDepthStencilState();
			//PBR Properties
			parameters.Albedo = glm::vec4(mat->Albedo, 1.0f);
			parameters.PBRParameters = glm::vec4(mat->Metallic, mat->Roughness, mat->AO, 1.0f);
		}
		else
		{
			parameters.Albedo = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);// default 
			parameters.PBRParameters = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);// default 
			PSOInit.PrimitiveType = EPrimitiveType::PT_TriangleList;
		}

		if (PSOInitializer.BoundShaderState.IsBound())
		{
			PSOInit.BoundShaderState.PixelShaderRHI = PSOInitializer.BoundShaderState.PixelShaderRHI;
			PSOInit.BoundShaderState.VertexShaderRHI = PSOInitializer.BoundShaderState.VertexShaderRHI;
			PSOInit.BoundShaderState.VertexDeclarationRHI = PSOInitializer.BoundShaderState.VertexDeclarationRHI;
		}

		if (PSOInitializer.DepthStencilState)
		{
			PSOInit.DepthStencilState = PSOInitializer.DepthStencilState;
		}

		if (PSOInitializer.RasterizerState)
		{
			PSOInit.RasterizerState = PSOInitializer.RasterizerState;
		}

		if (PSOInitializer.BlendState)
		{
			PSOInit.BlendState = PSOInitializer.BlendState;
		}

		UniformBuffer->ObjectUniformBuffer->UpdateUniformBufferImmediate(parameters);

		RHICmdList->SetGraphicsPipelineState(PSOInit);

		// Set VertexBuffer and IndexBuffer
		RHICmdList->SetIndexBuffer(staticMeshComp.GetRenderMesh()->GetIndexBuffer());
		RHICmdList->SetVertexBuffer(0, staticMeshComp.GetRenderMesh()->GetVertexBuffer());
		// Set Textures

		if (staticMeshComp.GetRenderMesh()->GetMaterial())
		{
			for (int i = 0; i < staticMeshComp.GetRenderMesh()->GetMaterial()->GetTextures().size(); i++)
			{
				RHICmdList->SetTexture(staticMeshComp.GetRenderMesh()->GetMaterial()->GetTextureStartSlot() + i, staticMeshComp.GetRenderMesh()->GetMaterial()->GetTextures()[i]);
			}
		}

		//Debug
		EnvironmentComponent& envComp = entity.GetComponent<EnvironmentComponent>();
		
		if (envComp.bDebugShowIBLType == 1)
		{
			RHICmdList->SetTexture(0, envComp.GetEnvDiffuseIrradiance());
		}
		else if (envComp.bDebugShowIBLType == 2)
		{
			RHICmdList->SetTexture(0, envComp.GetEnvSpecularPrefilter());
		}
		// Draw
		RHICmdList->DrawIndexPrimitive(0, 0, staticMeshComp.GetRenderMesh()->GetIndexCount() / 3);
	}

	void Renderer::OnResize(uint32_t newWidth, uint32_t newHeight)
	{
		m_Viewport.Width = (float)newWidth;
		m_Viewport.Height = (float)newHeight;
		
		m_SceneRenderTargets->OnResize(newWidth, newHeight, this);

		if(m_World->GetMainCamera())
		{
			m_World->GetMainCamera().GetComponent<CameraComponent>().SetViewportSize(newWidth, newHeight);
		}
	}
	void Renderer::Tick(float deltaTime)
	{
		std::vector<Entity> entitys = m_World->GetAllEntities();
		
		// classify the entity types
		gizmoDebugEntitys.clear();
		environmentEntitys.clear();
		normalEntitys.clear();
		lightEntitys.clear();

		for(int i = 0;i < entitys.size(); i++)
		{
			if(entitys[i].IsGizmo())
			{
				gizmoDebugEntitys.emplace_back(entitys[i]);
			}
			else if(entitys[i].HasComponent<EnvironmentComponent>())
			{
				environmentEntitys.emplace_back(entitys[i]);
			}
			else if(entitys[i].HasComponent<DirectionalLightComponent>())
			{
				lightEntitys.emplace_back(entitys[i]);
			}
			else
			{
				normalEntitys.emplace_back(entitys[i]);
			}
		}

		PreRender(deltaTime);

		ViewInfo viewinfo;
		viewinfo.ViewSize = m_Viewport;

		if (m_ShadingPath == EShadingPath::Forward)
		{
			m_ShadingRenderer = CreateRef<ForwardShadingRenderer>(viewinfo);
		}
		else if (m_ShadingPath == EShadingPath::Deferred)
		{
			m_ShadingRenderer = CreateRef<DeferredShadingRenderer>(viewinfo);
		}
		
		m_ShadingRenderer->Render(m_RHICommandList);
	}

	void Renderer::PreRender(float deltaTime)
	{
		// EquirectangularToCubeMap if exist
		for (int i = 0; i < environmentEntitys.size(); i++)
		{
			EnvironmentComponent& envComp = environmentEntitys[i].GetComponent<EnvironmentComponent>();
			if (envComp.m_EnvIsEquirectangular)
			{
				EnvEquirectangularToCubeMap(envComp);
				envComp.m_EnvIsEquirectangular = false;
			}
		}

		static bool bHasPreComputeIBL = false;
		// Image-Base-Lighting
		if (!bHasPreComputeIBL)
		{
			PreComputeIBL(environmentEntitys);
			bHasPreComputeIBL = true;
		}
		
	}
	
	void Renderer::UpdateViewUniformBuffer(Ref<RHICommandList> RHICmdList, Entity mainCameraEntity)
	{
		SceneUniformBuffers* UniformBuffer = SceneUniformBuffers::Get();
		if (!UniformBuffer || !Renderer::Get())
		{
			return;
		}
		RHICmdList->SetUniformBuffer(UniformBuffer->ViewUniformBuffer->GetSlotIndex(),
			EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
			UniformBuffer->ViewUniformBuffer->UniformBuffer());
		
		ViewUniformParameters parameters;
		static float accTime = 0;
		static float preRColorValue = 1.0f;
		static float preGColorValue = 0.0f;
		static float preBColorValue = 0.0f;
		accTime += Renderer::Get()->GetEngine()->GetSystem<Timer>()->GetDeltaTimeSec();
		if(accTime > 1.0f)
		{
			accTime = 0;
			preRColorValue = glm::linearRand(0, 1);
		}

		TransformComponent& transformComp = mainCameraEntity.GetComponent<TransformComponent>();
		CameraComponent& mainCameraComp = mainCameraEntity.GetComponent<CameraComponent>();
		glm::mat4 cameraTransform = transformComp.GetTransform();
		parameters.ViewMatrix = mainCameraComp.GetViewMatrix(); //glm::inverse(transformComp.GetTransform());
		parameters.InverseViewMatrix = glm::inverse(parameters.ViewMatrix);
		parameters.ProjectionMatrix = mainCameraComp.GetProjectionMatrix();
		parameters.InverseProjectionMatrix = glm::inverse(parameters.ProjectionMatrix);
		parameters.ViewProjectionMatrix = parameters.ProjectionMatrix * parameters.ViewMatrix;
		parameters.CameraWorldPosition = glm::vec4(transformComp.Position, 1.0f);
		//glm::vec4 debugPoint1 = parameters.ViewMatrix * glm::vec4(-0.5f, 0.5f, 1.0f, 1.0f);
		//glm::vec4 debugPoint = parameters.ProjectionMatrix * debugPoint1;
		
		parameters.TestColor = glm::vec3(preRColorValue,accTime,accTime);
		UniformBuffer->ViewUniformBuffer->UpdateUniformBufferImmediate(parameters);
	}

	void Renderer::UpdateLightUniformBuffer(Ref<RHICommandList> RHICmdList, const std::vector<Entity>& lightEntitys)
	{
		SceneUniformBuffers* UniformBuffer = SceneUniformBuffers::Get();
		if (!UniformBuffer || !Renderer::Get())
		{
			return;
		}
		RHICmdList->SetUniformBuffer(UniformBuffer->LightUniformBuffer->GetSlotIndex(),
        EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
			UniformBuffer->LightUniformBuffer->UniformBuffer());

		LightUniformParameters parameters;
		for(int i = 0;i < lightEntitys.size(); i++)
		{
			if(lightEntitys[i].HasComponent<DirectionalLightComponent>())
			{
				const DirectionalLightComponent& directionalLightComp = lightEntitys[i].GetComponent<DirectionalLightComponent>();
				
				parameters.DirectionalLightColor =
					glm::vec4(directionalLightComp.GetLightColor() * directionalLightComp.GetLightIntensity(), 1.0f);

				parameters.DirectionalLightDir = glm::vec4(directionalLightComp.GetLightDirection(), 1.0f);
			}
		}
		UniformBuffer->LightUniformBuffer->UpdateUniformBufferImmediate(parameters);
	}

	void Renderer::DrawFullScreenQuad(Ref<RHICommandList> RHICmdList, FullScreenUniformParameters fullScreenParameter,
		std::vector<std::shared_ptr<RHITexture>> Textures /*= std::vector<std::shared_ptr<RHITexture>>()*/)
	{
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
		static std::shared_ptr<RHIVertexShader> FullScreenVertexShader = nullptr;
		static std::shared_ptr<RHIPixelShader> FullScreenPixelShader = nullptr;
		static std::shared_ptr<RHIVertexDeclaration> FullScreenVertexDeclaration = nullptr;
		if (FullScreenVertexShader == nullptr)
		{
			RHIShaderCreateInfo shaderCreateInfo;
			FullScreenVertexShader = RHICreateVertexShader("Assets/Shaders/SimpleFullScreenVertex.hlsl", "MainVS", shaderCreateInfo);
			FullScreenPixelShader = RHICreatePixelShader("Assets/Shaders/SimpleFullScreenPixel.hlsl", "MainPS", shaderCreateInfo);
			if (GlobalRenderResources::GetInstance())
			{
				FullScreenVertexDeclaration = RHICreateVertexDeclaration(FullScreenVertexShader, GlobalRenderResources::GetInstance()->StandardMeshVertexDeclarationElementList);
			}
		}

		PixelShaderUtils::DrawFullScreenQuad(RHICmdList, FullScreenVertexDeclaration, FullScreenVertexShader, FullScreenPixelShader, Textures);
	}


	//--------------------------IBL--------------------------------
	void Renderer::EnvEquirectangularToCubeMap(EnvironmentComponent& envComp)
	{
		Ref<RHITexture2D> EnvEquirectangularTex = envComp.m_EnvEquirectangularTexture;
		Ref<RHITextureCube> EnvTexture = envComp.GetEnvironmentTexture();

		Viewport viewport(0, 0, EnvTexture->GetSizeX(), EnvTexture->GetSizeY());
		m_RHICommandList->SetViewport(viewport);

		// Set Render Target
		glm::mat4 captureViews[6];
		captureViews[0] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 90, 0))));
		captureViews[1] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, -90, 0))));
		captureViews[2] = glm::toMat4(glm::quat(glm::radians(glm::vec3(-90, 0, 0))));
		captureViews[3] = glm::toMat4(glm::quat(glm::radians(glm::vec3(90, 0, 0))));
		captureViews[4] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 0, 0))));
		captureViews[5] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 180, 0))));

		// DrawFullScreen
		for (int j = 0; j < 6; j++)
		{
			m_RHICommandList->SetRenderTarget(EnvTexture, j, nullptr);
			m_RHICommandList->SetTexture(0, EnvEquirectangularTex);
			m_RHICommandList->RHIClearRenderTarget(EnvTexture, j, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				nullptr);
			// DrawFullScreen
			FullScreenUniformParameters fullScreenParameter;
			fullScreenParameter.LocalToWorldMatrix = captureViews[j];
			// Update ObjectBuffer
			m_RHICommandList->SetUniformBuffer(m_SceneUniformBuffers->FullScreenUniformBuffer->GetSlotIndex(),
				EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
				m_SceneUniformBuffers->FullScreenUniformBuffer->UniformBuffer());

			m_SceneUniformBuffers->FullScreenUniformBuffer->UpdateUniformBufferImmediate(fullScreenParameter);
			//
			static std::shared_ptr<RHIVertexShader> FullScreenVertexShader = nullptr;
			static std::shared_ptr<RHIPixelShader> FullScreenPixelShader = nullptr;
			static std::shared_ptr<RHIVertexDeclaration> FullScreenVertexDeclaration = nullptr;
			if (FullScreenVertexShader == nullptr)
			{
				RHIShaderCreateInfo shaderCreateInfo;
				FullScreenVertexShader = RHICreateVertexShader("Assets/Shaders/EquirectangularToCubeMapVertex.hlsl", "MainVS", shaderCreateInfo);
				FullScreenPixelShader = RHICreatePixelShader("Assets/Shaders/EquirectangularToCubeMapPixel.hlsl", "MainPS", shaderCreateInfo);
				if (GlobalRenderResources::GetInstance())
				{
					FullScreenVertexDeclaration = RHICreateVertexDeclaration(FullScreenVertexShader, GlobalRenderResources::GetInstance()->StandardMeshVertexDeclarationElementList);
				}
			}
			PixelShaderUtils::DrawFullScreenQuad(m_RHICommandList, FullScreenVertexDeclaration, FullScreenVertexShader, FullScreenPixelShader);
		}
		m_RHICommandList->SetTexture(0, EnvEquirectangularTex);
	}

	void Renderer::PreComputeIBL(std::vector<Entity>& environmentEntitys)
	{
		/*
		// Set Render Target
		m_RHICommandList->SetViewport(m_Viewport);
		m_RHICommandList->SetRenderTarget(GetSceneRenderTargets()->GetSceneColorTexture(), GetSceneRenderTargets()->GetSceneDepthTexture());

		FullScreenUniformParameters fullScreenParameter;
		fullScreenParameter.LocalToWorldMatrix = glm::mat4();
		DrawFullScreenQuad(fullScreenParameter);
		return;
		*/

		// Pre-Compute-Diffuse-irradiance

		for (int i = 0; i < environmentEntitys.size(); i++)
		{
			EnvironmentComponent& envComp = environmentEntitys[i].GetComponent<EnvironmentComponent>();
			envComp.InitEnvDiffuseIrradianceTexture();

			//
			Ref<RHITextureCube> IrradianceDiffuseTex = envComp.GetEnvDiffuseIrradiance();

			// Set Render Target
			Viewport viewport(0, 0, IrradianceDiffuseTex->GetSizeX(), IrradianceDiffuseTex->GetSizeY());
			m_RHICommandList->SetViewport(viewport);
			glm::mat4 captureViews[6];
			captureViews[0] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 90, 0))));
			captureViews[1] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, -90, 0))));
			captureViews[2] = glm::toMat4(glm::quat(glm::radians(glm::vec3(-90, 0, 0))));
			captureViews[3] = glm::toMat4(glm::quat(glm::radians(glm::vec3(90, 0, 0))));
			captureViews[4] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 0, 0))));
			captureViews[5] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 180, 0))));
			//captureViews[5] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 0, 0))));

			//PreFilter Env Cubemap
			StaticMeshComponent& staticMeshComp = environmentEntitys[i].GetComponent<StaticMeshComponent>();
			/*
			* TODO:
			* caution: we should SetTexture after SetRenderTarget. if the texture is RenderTarget and bound to ShaderResource.
			* we need unbound rendertarget.so we should SetTexture after SetRenderTarget
			*/
			//m_RHICommandList->SetTexture(0, envComp.GetEnvironmentTexture());

			for (int j = 0; j < 6; j++)
			{
				m_RHICommandList->SetRenderTarget(IrradianceDiffuseTex, j, nullptr);
				m_RHICommandList->SetTexture(0, envComp.GetEnvironmentTexture());
				m_RHICommandList->RHIClearRenderTarget(IrradianceDiffuseTex, j, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
					nullptr);
				// DrawFullScreen
				FullScreenUniformParameters fullScreenParameter;
				fullScreenParameter.LocalToWorldMatrix = captureViews[j];
				ComputePreDiffuseIrradiance(fullScreenParameter);
			}
		}

		// PreFilter for Specular
		for (int i = 0; i < environmentEntitys.size(); i++)
		{
			EnvironmentComponent& envComp = environmentEntitys[i].GetComponent<EnvironmentComponent>();
			envComp.InitEnvSpecularPrefilterTexture();
			//
			Ref<RHITextureCube> SpecularPrefilterTex = envComp.GetEnvSpecularPrefilter();

			// Set Render Target
			uint32_t PreFilterSizeX = SpecularPrefilterTex->GetSizeX();
			uint32_t PreFilterSizeY = SpecularPrefilterTex->GetSizeY();
			ERHIPixelFormat tempPrefilterFormat = SpecularPrefilterTex->GetPixelFormat();
			unsigned int maxMipLevels = 5;
			for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
			{
				unsigned int mipWidth = PreFilterSizeX * std::pow(0.5, mip);
				unsigned int mipHeight = PreFilterSizeY * std::pow(0.5, mip);
				std::vector<Ref<RHITexture2D>> tempPrefilterTexures;
				RHIResourceCreateInfo createInfo;
				tempPrefilterTexures.emplace_back(RHICreateTexture2D(mipWidth, mipHeight, tempPrefilterFormat, 1,
					RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo));
				tempPrefilterTexures.emplace_back(RHICreateTexture2D(mipWidth, mipHeight, tempPrefilterFormat, 1,
					RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo));
				tempPrefilterTexures.emplace_back(RHICreateTexture2D(mipWidth, mipHeight, tempPrefilterFormat, 1,
					RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo));
				tempPrefilterTexures.emplace_back(RHICreateTexture2D(mipWidth, mipHeight, tempPrefilterFormat, 1,
					RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo));
				tempPrefilterTexures.emplace_back(RHICreateTexture2D(mipWidth, mipHeight, tempPrefilterFormat, 1,
					RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo));
				tempPrefilterTexures.emplace_back(RHICreateTexture2D(mipWidth, mipHeight, tempPrefilterFormat, 1,
					RHI_TexCreate_ShaderResource | RHI_TexCreate_RenderTargetable, createInfo));

				Viewport viewport(0, 0, mipWidth, mipHeight);
				m_RHICommandList->SetViewport(viewport);
				glm::mat4 captureViews[6];
				captureViews[0] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 90, 0))));
				captureViews[1] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, -90, 0))));
				captureViews[2] = glm::toMat4(glm::quat(glm::radians(glm::vec3(-90, 0, 0))));
				captureViews[3] = glm::toMat4(glm::quat(glm::radians(glm::vec3(90, 0, 0))));
				captureViews[4] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 0, 0))));
				captureViews[5] = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 180, 0))));

				//PreFilter Env Cubemap
				StaticMeshComponent& staticMeshComp = environmentEntitys[i].GetComponent<StaticMeshComponent>();
				m_RHICommandList->SetTexture(0, staticMeshComp.GetRenderMesh()->GetMaterial()->GetTextures()[0]);
				CustomDataFloat4UniformParameters customDataFloat4Parameter;
				const float roughness = float(mip) / float(maxMipLevels - 1);
				customDataFloat4Parameter.CustomData0.x = roughness;
				for (int j = 0; j < 6; j++)
				{
					m_RHICommandList->SetRenderTarget(tempPrefilterTexures[j], nullptr);
					m_RHICommandList->RHIClearRenderTarget(tempPrefilterTexures[j], glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
						nullptr);
					// DrawFullScreen
					FullScreenUniformParameters fullScreenParameter;
					fullScreenParameter.LocalToWorldMatrix = captureViews[j];
					ComputePreFilterSepcualr(fullScreenParameter, customDataFloat4Parameter);
				}
				// Set the Mip Texture
				m_RHICommandList->SetMipTexture(SpecularPrefilterTex, mip, mipWidth, mipHeight, tempPrefilterTexures);
			}
		}

		// Specular BRDF Integrate
		for (int i = 0; i < environmentEntitys.size(); i++)
		{
			EnvironmentComponent& envComp = environmentEntitys[i].GetComponent<EnvironmentComponent>();
			envComp.InitEnvSpecularIntegrateBRDFTexture();
			Ref<RHITexture2D> SpecularIntegrateBRDFTex = envComp.GetEnvSpecularIntegrateBRDF();
			Viewport viewport(0, 0, SpecularIntegrateBRDFTex->GetSizeX(), SpecularIntegrateBRDFTex->GetSizeY());
			m_RHICommandList->SetViewport(viewport);
			m_RHICommandList->SetRenderTarget(SpecularIntegrateBRDFTex, nullptr);
			m_RHICommandList->RHIClearRenderTarget(SpecularIntegrateBRDFTex, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				nullptr);
			// DrawFullScreen
			FullScreenUniformParameters fullScreenParameter;
			fullScreenParameter.LocalToWorldMatrix = glm::mat4();
			ComputePreIntegrateBRDF(fullScreenParameter);
		}

	}

	void Renderer::ComputePreDiffuseIrradiance(FullScreenUniformParameters fullScreenParameter)
	{
		// Update ObjectBuffer
		m_RHICommandList->SetUniformBuffer(m_SceneUniformBuffers->FullScreenUniformBuffer->GetSlotIndex(),
			EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
			m_SceneUniformBuffers->FullScreenUniformBuffer->UniformBuffer());

		m_SceneUniformBuffers->FullScreenUniformBuffer->UpdateUniformBufferImmediate(fullScreenParameter);
		//
		static std::shared_ptr<RHIVertexShader> FullScreenVertexShader = nullptr;
		static std::shared_ptr<RHIPixelShader> FullScreenPixelShader = nullptr;
		static std::shared_ptr<RHIVertexDeclaration> FullScreenVertexDeclaration = nullptr;
		if (FullScreenVertexShader == nullptr)
		{
			RHIShaderCreateInfo shaderCreateInfo;
			FullScreenVertexShader = RHICreateVertexShader("Assets/Shaders/ComputeDiffuseIrradianceVertex.hlsl", "MainVS", shaderCreateInfo);
			FullScreenPixelShader = RHICreatePixelShader("Assets/Shaders/ComputeDiffuseIrradiancePixel.hlsl", "MainPS", shaderCreateInfo);
			if (GlobalRenderResources::GetInstance())
			{
				FullScreenVertexDeclaration = RHICreateVertexDeclaration(FullScreenVertexShader, GlobalRenderResources::GetInstance()->StandardMeshVertexDeclarationElementList);
			}
		}
		PixelShaderUtils::DrawFullScreenQuad(m_RHICommandList, FullScreenVertexDeclaration, FullScreenVertexShader, FullScreenPixelShader);

	}

	void Renderer::ComputePreFilterSepcualr(FullScreenUniformParameters fullScreenParameter, CustomDataFloat4UniformParameters customDataFloat4Parameter)
	{
		// Update ObjectBuffer
		m_RHICommandList->SetUniformBuffer(m_SceneUniformBuffers->FullScreenUniformBuffer->GetSlotIndex(),
			EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
			m_SceneUniformBuffers->FullScreenUniformBuffer->UniformBuffer());
		m_SceneUniformBuffers->FullScreenUniformBuffer->UpdateUniformBufferImmediate(fullScreenParameter);

		m_RHICommandList->SetUniformBuffer(m_SceneUniformBuffers->CustomDataFloat4UniformBuffer->GetSlotIndex(),
			EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
			m_SceneUniformBuffers->CustomDataFloat4UniformBuffer->UniformBuffer());
		m_SceneUniformBuffers->CustomDataFloat4UniformBuffer->UpdateUniformBufferImmediate(customDataFloat4Parameter);

		//
		static std::shared_ptr<RHIVertexShader> FullScreenVertexShader = nullptr;
		static std::shared_ptr<RHIPixelShader> FullScreenPixelShader = nullptr;
		static std::shared_ptr<RHIVertexDeclaration> FullScreenVertexDeclaration = nullptr;
		if (FullScreenVertexShader == nullptr)
		{
			RHIShaderCreateInfo shaderCreateInfo;
			FullScreenVertexShader = RHICreateVertexShader("Assets/Shaders/ComputeSpecularPreFilterVertex.hlsl", "MainVS", shaderCreateInfo);
			FullScreenPixelShader = RHICreatePixelShader("Assets/Shaders/ComputeSpecularPreFilterPixel.hlsl", "MainPS", shaderCreateInfo);
			if (GlobalRenderResources::GetInstance())
			{
				FullScreenVertexDeclaration = RHICreateVertexDeclaration(FullScreenVertexShader, GlobalRenderResources::GetInstance()->StandardMeshVertexDeclarationElementList);
			}
		}
		PixelShaderUtils::DrawFullScreenQuad(m_RHICommandList, FullScreenVertexDeclaration, FullScreenVertexShader, FullScreenPixelShader);
	}

	void Renderer::ComputePreIntegrateBRDF(FullScreenUniformParameters fullScreenParameter)
	{
		// Update ObjectBuffer
		m_RHICommandList->SetUniformBuffer(m_SceneUniformBuffers->FullScreenUniformBuffer->GetSlotIndex(),
			EUniformBufferUsageScope::UBUS_Vertex | EUniformBufferUsageScope::UBUS_Pixel,
			m_SceneUniformBuffers->FullScreenUniformBuffer->UniformBuffer());
		m_SceneUniformBuffers->FullScreenUniformBuffer->UpdateUniformBufferImmediate(fullScreenParameter);

		static std::shared_ptr<RHIVertexShader> FullScreenVertexShader = nullptr;
		static std::shared_ptr<RHIPixelShader> FullScreenPixelShader = nullptr;
		static std::shared_ptr<RHIVertexDeclaration> FullScreenVertexDeclaration = nullptr;
		if (FullScreenVertexShader == nullptr)
		{
			RHIShaderCreateInfo shaderCreateInfo;
			FullScreenVertexShader = RHICreateVertexShader("Assets/Shaders/ComputeSpecularIntegrateBRDFVertex.hlsl", "MainVS", shaderCreateInfo);
			FullScreenPixelShader = RHICreatePixelShader("Assets/Shaders/ComputeSpecularIntegrateBRDFPixel.hlsl", "MainPS", shaderCreateInfo);
			if (GlobalRenderResources::GetInstance())
			{
				FullScreenVertexDeclaration = RHICreateVertexDeclaration(FullScreenVertexShader, GlobalRenderResources::GetInstance()->StandardMeshVertexDeclarationElementList);
			}
		}
		PixelShaderUtils::DrawFullScreenQuad(m_RHICommandList, FullScreenVertexDeclaration, FullScreenVertexShader, FullScreenPixelShader);
	}
	//--------------------------IBL--------------------------------
}
