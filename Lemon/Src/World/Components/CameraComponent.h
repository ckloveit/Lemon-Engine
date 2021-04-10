#pragma once
#include "Core/Core.h"
#include "IComponent.h"
#include <glm/glm.hpp>

namespace Lemon
{
    class LEMON_API CameraComponent : public IComponent
    {
     public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };

        CameraComponent();
        ~CameraComponent() = default;

        void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);
        void SetViewportSize(uint32_t width, uint32_t height);

        float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
        void SetPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
        void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

        float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
        float GetOrthographicFarClip() const { return m_OrthographicFar; }
        void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

        ProjectionType GetProjectionType() const { return m_ProjectionType; }
        void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		//=Getter
        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const;

		//Debug InputHandle
		void ProcessInputSystem(float deltaTime);

     private:
        void RecalculateProjection();   
     private:
        ProjectionType m_ProjectionType = ProjectionType::Perspective;   
        float m_AspectRatio = 1.0f;
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
        
        // Perspective Properties
        float m_PerspectiveFOV = glm::radians(90.0f);
        float m_PerspectiveNear = 0.1f;
        float m_PerspectiveFar = 1000.0f;

        // orthographic Properties
        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f;
        float m_OrthographicFar  = 1.0f;


		// Input
		glm::vec3 m_MovementSpeed;
    };
}