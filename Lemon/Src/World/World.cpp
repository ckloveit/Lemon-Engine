#include "LemonPCH.h"
#include "World.h"

#include "Components/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TransformComponent.h"
#include "entt/include/entt.hpp"
#include "RenderCore/Geometry/Cube.h"

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
			//cube.GetComponent<TransformComponent>().Position = { 0, 0, 0.5 };
			//cube.GetComponent<TransformComponent>().Rotation = { 20.0f, 0, 0 };

			StaticMeshComponent& staticMesh = cube.AddComponent<StaticMeshComponent>();
			Ref<Mesh> cubeMesh = CreateRef<Cube>();
			staticMesh.SetMesh(cubeMesh);
			cubeEntity = cube;
		}
    }
    void World::Tick(float deltaTime)
	{
		InitGeometry();

        if(cubeEntity)
        {
            //cubeEntity.GetComponent<TransformComponent>().Rotation += glm::vec3(0.0f, 0.0f, 1.0f);          
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

