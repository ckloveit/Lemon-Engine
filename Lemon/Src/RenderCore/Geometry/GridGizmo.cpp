#include "LemonPCH.h"
#include "GridGizmo.h"
#include <vector>
#include "World/World.h"
#include "World/Components/CameraComponent.h"
#include "World/Components/TransformComponent.h"
#include "World/Entity.h"

namespace Lemon
{
	GridGizmo::GridGizmo()
	{
		m_GridHeight = 200;
		m_GridWidth = 200;

		std::vector<StandardMeshVertex> vertices;
		std::vector<uint32_t> indices;
		BuildGrid(&vertices, &indices);
		BuileMesh(vertices, indices);

		// Shader And RHIResouce
		CreateShader<SF_Vertex>("Assets/Shaders/SimpleColorVertex.hlsl", "MainVS");
		CreateShader<SF_Pixel>("Assets/Shaders/SimpleColorPixel.hlsl", "MainPS");
		CreateRHIBuffers();
	}

	void GridGizmo::ComputeWorldAndScaleWithSnap(const CameraComponent& camera, glm::vec3& outTranslation, glm::vec3& outScale)
	{
		// To get the grid to feel infinite, it has to follow the camera,
		// but only by increments of the grid's spacing size. This gives the illusion 
		// that the grid never moves and if the grid is large enough, the user can't tell.
		const auto gridSpacing = 1.0f;
		const auto translation = glm::vec3
		(
			static_cast<int>(camera.m_Entity.GetComponent<TransformComponent>().Position.x / gridSpacing) * gridSpacing,
			0.0f,
			static_cast<int>(camera.m_Entity.GetComponent<TransformComponent>().Position.z / gridSpacing) * gridSpacing
		);
		outTranslation = translation;
		outScale = glm::vec3(gridSpacing, gridSpacing, gridSpacing);

		/*
				glm::mat4 worldMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(gridSpacing, gridSpacing, gridSpacing))
					* glm::translate(glm::mat4(1.0f), translation);

				return worldMatrix;*/
	}

	void GridGizmo::BuildGrid(std::vector<StandardMeshVertex>* vertices, std::vector<uint32_t>* indices)
	{
		const auto halfSizeW = int(m_GridWidth * 0.5f);
		const auto halfSizeH = int(m_GridHeight * 0.5f);

		for (auto j = -halfSizeH; j < halfSizeH; j++)
		{
			for (auto i = -halfSizeW; i < halfSizeW; i++)
			{
				// Become more transparent, the further out we go
				const auto alphaWidth = 1.0f - static_cast<float>(abs(j)) / static_cast<float>(halfSizeH);
				const auto alphaHeight = 1.0f - static_cast<float>(abs(i)) / static_cast<float>(halfSizeW);
				auto alpha = (alphaWidth + alphaHeight) * 0.5f;
				alpha = pow(alpha, 10.0f);

				// LINE 1
				// Upper left.
				auto positionX = static_cast<float>(i);
				auto positionZ = static_cast<float>(j + 1);
				vertices->emplace_back(glm::vec3(positionX, 0.0f, positionZ), glm::vec4(1.0f, 1.0f, 1.0f, alpha));

				// Upper right.
				positionX = static_cast<float>(i + 1);
				positionZ = static_cast<float>(j + 1);
				vertices->emplace_back(glm::vec3(positionX, 0.0f, positionZ), glm::vec4(1.0f, 1.0f, 1.0f, alpha));

				// LINE 2
				// Upper right.
				positionX = static_cast<float>(i + 1);
				positionZ = static_cast<float>(j + 1);
				vertices->emplace_back(glm::vec3(positionX, 0.0f, positionZ), glm::vec4(1.0f, 1.0f, 1.0f, alpha));

				// Bottom right.
				positionX = static_cast<float>(i + 1);
				positionZ = static_cast<float>(j);
				vertices->emplace_back(glm::vec3(positionX, 0.0f, positionZ), glm::vec4(1.0f, 1.0f, 1.0f, alpha));

				// LINE 3
				// Bottom right.
				positionX = static_cast<float>(i + 1);
				positionZ = static_cast<float>(j);
				vertices->emplace_back(glm::vec3(positionX, 0.0f, positionZ), glm::vec4(1.0f, 1.0f, 1.0f, alpha));

				// Bottom left.
				positionX = static_cast<float>(i);
				positionZ = static_cast<float>(j);
				vertices->emplace_back(glm::vec3(positionX, 0.0f, positionZ), glm::vec4(1.0f, 1.0f, 1.0f, alpha));

				// LINE 4
				// Bottom left.
				positionX = static_cast<float>(i);
				positionZ = static_cast<float>(j);
				vertices->emplace_back(glm::vec3(positionX, 0.0f, positionZ), glm::vec4(1.0f, 1.0f, 1.0f, alpha));

				// Upper left.
				positionX = static_cast<float>(i);
				positionZ = static_cast<float>(j + 1);
				vertices->emplace_back(glm::vec3(positionX, 0.0f, positionZ), glm::vec4(1.0f, 1.0f, 1.0f, alpha));
			}
		}

		for (uint32_t i = 0; i < vertices->size(); i++)
		{
			indices->emplace_back(i);
		}
	}




}