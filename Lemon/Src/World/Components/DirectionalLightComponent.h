#pragma once
#include "Core/Core.h"
#include "IComponent.h"
#include <glm/glm.hpp>
#include "RenderCore/Mesh.h"

namespace Lemon
{
    class LEMON_API DirectionalLightComponent : public IComponent
    {
    public:
        DirectionalLightComponent() {}

        glm::vec3 GetLightDirection() const;

        void SetLightIntensity(float lightIntensity) { m_LightIntensity = lightIntensity; }
        float GetLightIntensity() const { return m_LightIntensity; }

        void SetLightColor(glm::vec3 color) {m_LightColor = color;}
        glm::vec3 GetLightColor() const { return m_LightColor; }
    private:
        float m_LightIntensity = 1.0f;
        glm::vec3 m_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    };
}