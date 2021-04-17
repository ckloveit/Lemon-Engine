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

        Entity CreateEntity(const std::string& name = std::string(), bool bIsGizmoDebug = false);
		void DestroyEntity(Entity entity);
        
        Entity GetMainCamera() const { return MainCameraEntity; }
                
        std::vector<Entity> GetAllEntities() const;
        std::vector<Entity> GetAllEnvironmentEntitys() const;
		std::vector<Entity> GetGizmoDebugEntitys() const;
    private:
        void CreateMainCamera();
        void InitRenderGeometry();
		void CreateEnvironment(float SkySphereRadius = 1000.0f);
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
