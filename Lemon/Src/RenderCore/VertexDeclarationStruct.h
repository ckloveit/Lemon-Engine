#pragma once
#include "Core/Core.h"
#include <glm/glm.hpp>

namespace Lemon
{
	#define MAX_MESH_TEXTURE_COORDS 2

	struct StandardMeshVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec2 Texcoords[MAX_MESH_TEXTURE_COORDS];
		StandardMeshVertex() = default;
		StandardMeshVertex(glm::vec3 InPosition, glm::vec4 InColor)
			: Position(InPosition)
			, Color(InColor)
			, Normal(glm::vec3(0, 0, 1))
			, Tangent(glm::vec3(1, 0, 0))
		{
			for (int i = 0; i < MAX_MESH_TEXTURE_COORDS; i++)
				Texcoords[i] = glm::vec2(0, 0);
		}
		StandardMeshVertex(glm::vec3 InPosition, glm::vec2 InUV0)
			: Position(InPosition)
			, Color(glm::vec4(1, 0, 0, 1))
			, Normal(glm::vec3(0, 0, 1))
			, Tangent(glm::vec3(1, 0, 0))
		{
			Texcoords[0] = InUV0;
			for (int i = 1; i < MAX_MESH_TEXTURE_COORDS; i++)
				Texcoords[i] = glm::vec2(0, 0);
		}
		StandardMeshVertex(glm::vec3 InPosition, glm::vec4 InColor, glm::vec3 InNormal, glm::vec3 InTangent, glm::vec2 InUV0)
			: Position(InPosition)
			, Color(InColor)
			, Normal(InNormal)
			, Tangent(InTangent)
		{
			Texcoords[0] = InUV0;
			for (int i = 1; i < MAX_MESH_TEXTURE_COORDS; i++)
				Texcoords[i] = glm::vec2(0, 0);
		}
	};




}