#include "LemonPCH.h"
#include "Quad.h"
#include <vector>

namespace Lemon
{
	Quad::Quad(float quadSize /*= 1.0f*/)
		:m_QuadSize(quadSize)
	{
		std::vector<StandardMeshVertex> vertices;
		std::vector<uint32_t> indices;
		BuildQuad(&vertices, &indices);
		BuileMesh(vertices, indices);

		// Shader And RHIResouce
		CreateShader<SF_Vertex>("Assets/Shaders/SimpleStandardVertex.hlsl", "MainVS");
		CreateShader<SF_Pixel>("Assets/Shaders/SimpleStandardPixel.hlsl", "MainPS");
		CreateRHIBuffers();
	}

	void Quad::BuildQuad(std::vector<StandardMeshVertex>* vertices, std::vector<uint32_t>* indices)
	{
		vertices->emplace_back(glm::vec3(-1.0f,  1.0f, 0.5f) * m_QuadSize, glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));
		vertices->emplace_back(glm::vec3(-1.0f, -1.0f, 0.5f) * m_QuadSize, glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
		vertices->emplace_back(glm::vec3( 1.0f, -1.0f, 0.5f) * m_QuadSize, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		vertices->emplace_back(glm::vec3( 1.0f,  1.0f, 0.5f) * m_QuadSize, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

		// 
		indices->emplace_back(0);
		indices->emplace_back(1);
		indices->emplace_back(2);
		indices->emplace_back(0);
		indices->emplace_back(2);
		indices->emplace_back(3);
	}




}