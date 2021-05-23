#pragma once
#include "Core/Core.h"
#include "Core/ISystem.h"
#include <entt/include/entt.hpp>
#include "Entity.h"
#include "RenderCore/RenderCore.h"

namespace Lemon
{
    class LEMON_API World : public ISystem
    {
        friend class Entity;
    public:
		World(Engine* engine);
		~World() = default;
        
        bool Initialize() override;
        void Tick(float deltaTime) override;

    	void EndOneFrame();

        Entity CreateEntity(const std::string& name = std::string(), bool bIsGizmoDebug = false);
		void DestroyEntity(Entity& entity);
        
        Entity GetMainCamera() const { return MainCameraEntity; }
                
        std::vector<Entity> GetAllEntities() const;
    private:
        void CreateMainCamera();
		void CreateEnvironment(float SkySphereRadius = 1000.0f);

    	//======Debug
        void InitRenderGeometry();
    	void CreateLight();
    	

		// Create Test Sphere
		void CreateTestSphere();
    private:
        entt::registry m_Registry;
        Entity MainCameraEntity;
		Entity GridGizmoEntity;
		Entity EnvironmentEntity;

        std::vector<Entity> m_Entitys;

        std::vector<Entity> m_EnvironmentEntitys;

		std::vector<Entity> m_GizmoDebugEntitys;

        //Just Debug
        Entity cubeEntity;

    };
    
}
