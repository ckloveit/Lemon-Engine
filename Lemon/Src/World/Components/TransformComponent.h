#pragma once
#include "Core/Core.h"
#include "IComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
namespace Lemon
{
    class LEMON_API TransformComponent : public IComponent
    {
	public:
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        // uint : degree
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position)
            : Position(position) {}
        glm::mat4 GetTransform() const
        {
			/*
            glm::mat4 rotation1 = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
            * glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
            * glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });
			glm::mat4 test10 = glm::rotate(glm::mat4(1.0f), 0.0f, { 0, 1, 0 });
			glm::mat4 test11 = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0, 1, 0 });
			glm::mat4 test12 = glm::rotate(glm::mat4(1.0f), 360.0f, { 0, 1, 0 });

			glm::mat4 test1 = glm::toMat4(glm::quat(glm::vec3(0, 0, 0)));
			glm::mat4 test2 = glm::toMat4(glm::quat(glm::vec3(0, glm::radians(90.0f), 0)));
			glm::mat4 test3 = glm::toMat4(glm::quat(glm::vec3(0, glm::radians(180.0f), 0)));
			glm::mat4 test4 = glm::toMat4(glm::quat(glm::vec3(0, 360, 0)));
			*/
            glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));

            return glm::translate(glm::mat4(1.0f), Position)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);
        }
    };
    
}