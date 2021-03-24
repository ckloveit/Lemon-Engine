#include "LemonPCH.h"
#include "Entity.h"

namespace Lemon
{
    Entity::Entity(entt::entity handle, World* world, const std::string& entityName)
        : m_EntityHandle(handle), m_World(world), m_entityName(entityName)
    {
        
    }

}