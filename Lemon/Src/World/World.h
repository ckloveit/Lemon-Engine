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

        Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);
        
        Entity GetMainCamera() const { return MainCameraEntity; }
                
        std::vector<Entity> GetAllEntities() const;
    private:
        void CreateMainCamera();
        void InitGeometry();
    private:
        entt::registry m_Registry;
        Entity MainCameraEntity;
		Entity GridGizmoEntity;


        std::vector<Entity> m_Entitys;

        //Just Debug
        Entity cubeEntity;
    };
    
}
