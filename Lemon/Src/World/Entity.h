#pragma once
#include "Core/Core.h"
#include <type_traits>
#include <entt/include/entt.hpp>

namespace Lemon
{
    class World;

#define CHECK_COMPONENT_VALID() 	constexpr bool bValid = std::is_base_of<IComponent, T>::value;\
									LEMON_CORE_ASSERT(bValid, "Component is not derived IComponent");

    class LEMON_API Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, World* world, const std::string& entityName);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
			CHECK_COMPONENT_VALID();
            LEMON_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            T& component = m_World->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			component.m_Entity = *this;
            return component;
        }

        template<typename T>
        T& GetComponent()
		{
			CHECK_COMPONENT_VALID();
			LEMON_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_World->m_Registry.get<T>(m_EntityHandle);
        }
		template<typename T>
		const T& GetComponent() const
		{
			CHECK_COMPONENT_VALID();
			LEMON_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_World->m_Registry.get<T>(m_EntityHandle);
		}

        template<typename T>
        void RemoveComponent()
		{
			CHECK_COMPONENT_VALID();
            LEMON_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            m_World->m_Registry.remove<T>(m_EntityHandle);
        }
        
        template<typename T>
        bool HasComponent()
        {
            return m_World->m_Registry.has<T>(m_EntityHandle);
        }
		template<typename T>
		bool HasComponent() const
		{
			return m_World->m_Registry.has<T>(m_EntityHandle);
		}
        // operator
        operator bool() const { return m_EntityHandle != entt::null && m_World != nullptr; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }
        operator entt::entity() const { return m_EntityHandle; }
        bool operator==(const Entity& other) const
        {
            return m_EntityHandle == other.m_EntityHandle && m_World == other.m_World;
        }
        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }
		World* GetWorld() const { return m_World; }

    	// Name
    	const std::string& GetName() const { return m_entityName; }
		std::string& GetName() {return m_entityName;}
		// Gizmo flag
		bool IsGizmo() const { return m_bIsGizmo; }
		void SetGizmo(bool bIsGizmo) { m_bIsGizmo = bIsGizmo; }

    	//
    	void MarkDestroy() { m_bIsDestroy = true; }
    	bool IsMarkDestroy() const { return m_bIsDestroy; }
    	
    private:
        entt::entity m_EntityHandle {0};
        World* m_World = nullptr;
        std::string m_entityName;
		bool m_bIsGizmo = false;
    	bool m_bIsDestroy = false;
    };
}

