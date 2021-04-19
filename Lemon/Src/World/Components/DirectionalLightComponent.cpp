#include "LemonPCH.h"
#include "DirectionalLightComponent.h"
#include "World/Entity.h"
#include "World/World.h"
#include "TransformComponent.h"

namespace Lemon
{
    glm::vec3 DirectionalLightComponent::GetLightDirection() const
    {
        if(m_Entity && m_Entity.HasComponent<TransformComponent>())
        {
            glm::quat rotation = glm::quat(glm::radians(m_Entity.GetComponent<TransformComponent>().Rotation));
            glm::vec3 dir = glm::rotate(rotation, glm::vec3(1.0f, 0.0f, 0.0f));
            return glm::normalize(dir);
        }
        return glm::vec3(1.0f, 0.0f, 0.0f);
    }
}
