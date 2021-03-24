#include "LemonPCH.h"
#include "CameraComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "World/World.h"
#include "World/Entity.h"
#include "TransformComponent.h"

namespace Lemon
{
    CameraComponent::CameraComponent()
    {
        RecalculateProjection();
    }

    void CameraComponent::SetOrthographic(float size, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Orthographic;
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;
		RecalculateProjection();
    }
    void CameraComponent::SetPerspective(float verticalFOV, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Perspective;
        m_PerspectiveFOV = verticalFOV;
        m_PerspectiveNear = nearClip;
        m_PerspectiveFar = farClip;
        RecalculateProjection();
    }
        
    void CameraComponent::SetViewportSize(uint32_t width, uint32_t height)
    {
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
    }

    
    void CameraComponent::RecalculateProjection()
    {
        if(m_ProjectionType == ProjectionType::Perspective)
        {
			// caution : for Direct3D the perspective need in LR and Zero to One 
            m_ProjectionMatrix = glm::perspectiveLH_ZO(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
        }
        else
        {
            float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

            m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight,
                orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
        }

    }


	const glm::mat4& CameraComponent::GetViewMatrix() const
	{
		if (m_Entity)
		{
			return glm::inverse(m_Entity.GetComponent<TransformComponent>().GetTransform());
		}
		return glm::mat4(1.0f);
	}

    
}
