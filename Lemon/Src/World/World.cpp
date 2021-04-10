#include "LemonPCH.h"
#include "World.h"

#include "Components/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TransformComponent.h"
#include "entt/include/entt.hpp"
#include "RenderCore/Geometry/Cube.h"
#include "RenderCore/Geometry/GridGizmo.h"
#include "RHI/RHIStaticStates.h"

namespace Lemon
{
    World::World(Engine* engine)
        :ISystem(engine)
    {
		    
	}
    
    Entity World::CreateEntity(const std::string& name)
    {
        Entity entity(m_Registry.create(), this, name);
        entity.AddComponent<TransformComponent>();
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
    
    void World::InitGeometry()
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
			cubeEntity = cube;

			cube = CreateEntity("Cube2");
			StaticMeshComponent& staticMesh1 = cube.AddComponent<StaticMeshComponent>();
			cubeMesh = CreateRef<Cube>();
			staticMesh1.SetMesh(cubeMesh);

			//GridGizmo
			GridGizmoEntity = CreateEntity("GridGizmo");
			StaticMeshComponent& gridMeshComp = GridGizmoEntity.AddComponent<StaticMeshComponent>();
			Ref<Mesh> gridMesh = CreateRef<GridGizmo>();
			gridMeshComp.SetMesh(gridMesh);
			Ref<RHIRasterizerState> rasterizerState = TStaticRasterizerState<RFM_Wireframe, RCM_None>::CreateRHI();
			gridMesh->SetRasterizerState(rasterizerState);
			gridMesh->SetPrimitiveType(EPrimitiveType::PT_LineList);
			// for mirror window, write RGBA, RGB = src.rgb * src.a + dst.rgb * (1 - src.a), A = src.a * 1 + dst.a * (1 - src a)
			Ref<RHIBlendState> translucencyBlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_SourceAlpha, BF_InverseSourceAlpha, BO_Add,
				BF_One, BF_InverseSourceAlpha>::CreateRHI();
			gridMesh->SetBlendState(translucencyBlendState);
		}
    }
    void World::Tick(float deltaTime)
	{
		InitGeometry();

        if(cubeEntity)
        {
            //cubeEntity.GetComponent<TransformComponent>().Rotation += glm::vec3(0.0f, 0.0f, 1.0f);          
        }

		if (MainCameraEntity)
		{
			MainCameraEntity.GetComponent<CameraComponent>().ProcessInputSystem(deltaTime);
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
    std::vector<Entity> World::GetAllEntities() const
    {
        return m_Entitys;
    }
}

