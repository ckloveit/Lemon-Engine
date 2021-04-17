#include "LemonPCH.h"
#include "World.h"

#include "Components/CameraComponent.h"
#include "Components/EnvironmentComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TransformComponent.h"
#include "Core/Engine.h"
#include "Core/Timer.h"
#include "entt/include/entt.hpp"
#include "RenderCore/Geometry/Cube.h"
#include "RenderCore/Geometry/Sphere.h"

#include "RenderCore/Geometry/GridGizmo.h"
#include "Renderer/Renderer.h"
#include "Resources/ResourceSystem.h"
#include "RHI/RHIStaticStates.h"

namespace Lemon
{
    World::World(Engine* engine)
        :ISystem(engine)
    {
		    
	}
    
    Entity World::CreateEntity(const std::string& name, bool bIsGizmoDebug /*= false*/)
    {
        Entity entity(m_Registry.create(), this, name);
        entity.AddComponent<TransformComponent>();
		entity.SetGizmo(bIsGizmoDebug);
        m_Entitys.emplace_back(entity);
        return entity;
    }
   
    void World::DestroyEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }
    
    bool World::Initialize()
    {
        CreateMainCamera();

        // Init
        return true;
    }
    
    void World::InitRenderGeometry()
    {
		static bool bHasInitGeometry = false;
		if (!bHasInitGeometry)
		{
			bHasInitGeometry = true;
			//Create Cube
			Entity cube = CreateEntity("Cube1");
			cube.GetComponent<TransformComponent>().Position = { 0, 0, 2.5 };
			//cube.GetComponent<TransformComponent>().Position = { 0, 0, 0.5 };
			//cube.GetComponent<TransformComponent>().Rotation = { 20.0f, 0, 0 };

			StaticMeshComponent& staticMesh = cube.AddComponent<StaticMeshComponent>();
			Ref<Mesh> cubeMesh = CreateRef<Cube>();
			staticMesh.SetMesh(cubeMesh);

			cube = CreateEntity("Cube2");
			StaticMeshComponent& staticMesh1 = cube.AddComponent<StaticMeshComponent>();
			cubeMesh = CreateRef<Cube>();
			staticMesh1.SetMesh(cubeMesh);
			cubeEntity = cube;

			Entity sphere = CreateEntity("Sphere");
			StaticMeshComponent& staticMesh2 = sphere.AddComponent<StaticMeshComponent>();
			Ref<Mesh> sphereMesh = CreateRef<Sphere>();
			staticMesh2.SetMesh(sphereMesh);

			//GridGizmo
			GridGizmoEntity = CreateEntity("GridGizmo", true);
			m_GizmoDebugEntitys.emplace_back(GridGizmoEntity);

			StaticMeshComponent& gridMeshComp = GridGizmoEntity.AddComponent<StaticMeshComponent>();
			Ref<Mesh> gridMesh = CreateRef<GridGizmo>();
			gridMeshComp.SetMesh(gridMesh);
			Ref<RHIRasterizerState> rasterizerState = TStaticRasterizerState<RFM_Wireframe, RCM_None>::CreateRHI();

			Ref<Material> renderMaterial = CreateRef<Material>();
			renderMaterial->SetRasterizerState(rasterizerState);
			renderMaterial->SetPrimitiveType(EPrimitiveType::PT_LineList);
			// for mirror window, write RGBA, RGB = src.rgb * src.a + dst.rgb * (1 - src.a), A = src.a * 1 + dst.a * (1 - src a)
			Ref<RHIBlendState> translucencyBlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_SourceAlpha, BF_InverseSourceAlpha, BO_Add,
				BF_One, BF_InverseSourceAlpha>::CreateRHI();
			renderMaterial->SetBlendState(translucencyBlendState);
			gridMesh->SetMaterial(renderMaterial);
			
			// Create Env
			CreateEnvironment();
		}
    }
    void World::Tick(float deltaTime)
	{
		InitRenderGeometry();

        if(cubeEntity)
        {
        	cubeEntity.GetComponent<TransformComponent>().Position.x = 5.0f * sin(0.4f * GetEngine()->GetTimer()->GetGameTime());
        	
            cubeEntity.GetComponent<TransformComponent>().Rotation += glm::vec3(0.0f, 0.0f, 0.1f);          
        }

		if (MainCameraEntity)
		{
			//MainCameraEntity.GetComponent<CameraComponent>().ProcessInputSystem(deltaTime);
		}

		if (GridGizmoEntity)
		{
			glm::vec3 translation;
			glm::vec3 scale;
			GridGizmo::ComputeWorldAndScaleWithSnap(MainCameraEntity.GetComponent<CameraComponent>(), translation, scale);
			GridGizmoEntity.GetComponent<TransformComponent>().Position = translation;
			GridGizmoEntity.GetComponent<TransformComponent>().Scale = scale;
		}

    }
    
    void World::CreateMainCamera()
    {
		MainCameraEntity = CreateEntity("MainCamera");
		MainCameraEntity.GetComponent<TransformComponent>().Rotation = glm::vec3(0, 180.0f, 0);
		MainCameraEntity.GetComponent<TransformComponent>().Position = glm::vec3(0, 0,  1.5f);
        CameraComponent& camera = MainCameraEntity.AddComponent<CameraComponent>();
        camera.SetProjectionType(CameraComponent::ProjectionType::Perspective);
    }

	void World::CreateEnvironment(float SkySphereRadius /* = 1000.0f*/)
	{
		EnvironmentEntity = CreateEntity("Environment");
    	// Create Static Mesh Component
		StaticMeshComponent& staticMesh = EnvironmentEntity.AddComponent<StaticMeshComponent>();
		Ref<Mesh> sphereMesh = CreateRef<Sphere>(SkySphereRadius, false);
		sphereMesh->CreateShader<SF_Vertex>("Assets/Shaders/SimpleEnvironmentCubeVertex.hlsl", "MainVS");
		sphereMesh->CreateShader<SF_Pixel>("Assets/Shaders/SimpleEnvironmentCubePixel.hlsl", "MainPS");
		sphereMesh->CreateRHIBuffers();
		staticMesh.SetMesh(sphereMesh);
    	
    	Ref<RHIRasterizerState> CullFrontRS = GetEngine()->GetSystem<Renderer>()->GetSceneRenderStates()->SolidCullFrontRasterizerState;
		Ref<Material> renderMaterial = CreateRef<Material>();
    	renderMaterial->SetRasterizerState(CullFrontRS);
		// Create Environment Component
    	EnvironmentComponent& EnvComp = EnvironmentEntity.AddComponent<EnvironmentComponent>();
		std::vector<std::string> envFilePaths;
		const auto dir_cubemaps = GetEngine()->GetSystem<ResourceSystem>()->GetAssetDataDirectory(Asset_Cubemaps) + "/";
    	envFilePaths.emplace_back(dir_cubemaps + "array/X+.tga");// Right
    	envFilePaths.emplace_back(dir_cubemaps + "array/X-.tga");// Left
    	envFilePaths.emplace_back(dir_cubemaps + "array/Y+.tga");// Up
    	envFilePaths.emplace_back(dir_cubemaps + "array/Y-.tga");// down
    	envFilePaths.emplace_back(dir_cubemaps + "array/Z-.tga");// back
    	envFilePaths.emplace_back(dir_cubemaps + "array/Z+.tga");// front
    	EnvComp.CreateFromFilePath(envFilePaths);
    	renderMaterial->GetTextures().emplace_back(EnvComp.GetEnvironmentTexture());
    	sphereMesh->SetMaterial(renderMaterial);
    	
    	m_EnvironmentEntitys.emplace_back(EnvironmentEntity);
    	
	}

    std::vector<Entity> World::GetAllEntities() const
    {
        return m_Entitys;
    }
	
	std::vector<Entity> World::GetAllEnvironmentEntitys() const
    {
	    return m_EnvironmentEntitys;
    }

	std::vector<Entity> World::GetGizmoDebugEntitys() const
	{
		return m_GizmoDebugEntitys;
	}
}

