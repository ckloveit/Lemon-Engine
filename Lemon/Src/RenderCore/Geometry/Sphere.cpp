#include "LemonPCH.h"
#include "Sphere.h"
#include <vector>
#include "Math/Math.h"

namespace Lemon
{
	Sphere::Sphere(float radius /*= 1.0f*/, bool bCompileDefaultShader /*= true*/, int slices /*= 20*/, int stacks /*= 20*/)
		:m_Radius(radius),m_Slices(slices),m_Stacks(stacks)
	{
		std::vector<StandardMeshVertex> vertices;
		std::vector<uint32_t> indices;
		BuildSphere(&vertices, &indices);
		BuileMesh(vertices, indices);

		// Shader And RHIResouce
		if (bCompileDefaultShader)
		{
			CreateShader<SF_Vertex>("Assets/Shaders/SimpleStandardVertex.hlsl", "MainVS");
			CreateShader<SF_Pixel>("Assets/Shaders/SimpleStandardPixel.hlsl", "MainPS");
			CreateRHIBuffers();
		}
	}

	void Sphere::BuildSphere(std::vector<StandardMeshVertex>* vertices, std::vector<uint32_t>* indices)
	{
		glm::vec3 normal = glm::vec3(0, 1, 0);
		glm::vec3 tangent = glm::vec3(1, 0, 0);
		vertices->emplace_back(glm::vec3(0, m_Radius, 0), glm::vec4(glm::vec3(0, m_Radius, 0), 1.0f), normal, tangent, glm::vec2(0, 0));

		const float phiStep = Math::PI / m_Stacks;
		const float thetaStep = 2.0f * Math::PI / m_Slices;

		for (int i = 1; i <= m_Stacks - 1; i++)
		{
			const float phi = i * phiStep;
			for (int j = 0; j <= m_Slices; j++)
			{
				const float theta = j * thetaStep;
				glm::vec3 p = glm::vec3(
					(m_Radius * sin(phi) * cos(theta)),
					(m_Radius * cos(phi)),
					(m_Radius * sin(phi) * sin(theta))
				);

				glm::vec3 t = glm::vec3(-m_Radius * sin(phi) * sin(theta), 0, m_Radius * sin(phi) * cos(theta));
				t = glm::normalize(t);
				glm::vec3 n = glm::normalize(p);
				glm::vec2 uv = glm::vec2(theta / (Math::PI * 2), phi / Math::PI);
				vertices->emplace_back(p, glm::vec4(p, 1.0f), n, t, uv);
			}
		}
		normal = glm::vec3(0, -1, 0);
		tangent = glm::vec3(1, 0, 0);
		vertices->emplace_back(glm::vec3(0, -m_Radius, 0), glm::vec4(glm::vec3(0, m_Radius, 0), 1.0f), normal, tangent, glm::vec2(0, 1));
		
		for (int i = 1; i <= m_Slices; i++)
		{
			indices->emplace_back(0);
			//indices->emplace_back(i + 1);
			indices->emplace_back(i);
			indices->emplace_back(i + 1);
		}
		int baseIndex = 1;
		const int ringVertexCount = m_Slices + 1;
		for (int i = 0; i < m_Stacks - 2; i++)
		{
			for (int j = 0; j < m_Slices; j++)
			{
				indices->emplace_back(baseIndex + i * ringVertexCount + j);
				//indices->emplace_back(baseIndex + i * ringVertexCount + j + 1);
				indices->emplace_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices->emplace_back(baseIndex + i * ringVertexCount + j + 1);
				
				indices->emplace_back(baseIndex + (i + 1) * ringVertexCount + j);
				//indices->emplace_back(baseIndex + i * ringVertexCount + j + 1);
				indices->emplace_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
				indices->emplace_back(baseIndex + i * ringVertexCount + j + 1);
			}
		}
		int southPoleIndex = (int)vertices->size() - 1;
		baseIndex = southPoleIndex - ringVertexCount;
		for (int i = 0; i < m_Slices; i++)
		{
			indices->emplace_back(southPoleIndex);
			//indices->emplace_back(baseIndex + i);
			indices->emplace_back(baseIndex + i + 1);
			indices->emplace_back(baseIndex + i);
		}

	}




}