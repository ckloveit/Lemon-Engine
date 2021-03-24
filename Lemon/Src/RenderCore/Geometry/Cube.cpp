#include "LemonPCH.h"
#include "Cube.h"
#include <vector>

namespace Lemon
{
	Cube::Cube(float cubeSize /*= 1.0f*/)
		:m_CubeSize(cubeSize)
	{
		std::vector<StandardMeshVertex> vertices;
		std::vector<uint32_t> indices;
		BuildCube(&vertices, &indices);
		BuileMesh(vertices, indices);

		// Shader And RHIResouce
		CreateShader<SF_Vertex>("Assets/Shaders/SimpleStandardVertex.hlsl", "MainVS");
		CreateShader<SF_Pixel>("Assets/Shaders/SimpleStandardPixel.hlsl", "MainPS");
		CreateRHIBuffers();
	}

	void Cube::BuildCube(std::vector<StandardMeshVertex>* vertices, std::vector<uint32_t>* indices)
	{
		vertices->emplace_back(glm::vec3(-0.5f, 0.5f, 0.5f) * m_CubeSize, glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));
		vertices->emplace_back(glm::vec3(-0.5f, 0.5f, -0.5f) * m_CubeSize, glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
		vertices->emplace_back(glm::vec3(0.5f, 0.5f, -0.5f) * m_CubeSize, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		vertices->emplace_back(glm::vec3(0.5f, 0.5f, 0.5f) * m_CubeSize, glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));

		vertices->emplace_back(glm::vec3(-0.5f, -0.5f, 0.5f) * m_CubeSize, glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f));
		vertices->emplace_back(glm::vec3(-0.5f, -0.5f, -0.5f) * m_CubeSize, glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
		vertices->emplace_back(glm::vec3(0.5f, -0.5f, -0.5f) * m_CubeSize, glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));
		vertices->emplace_back(glm::vec3(0.5f, -0.5f, 0.5f) * m_CubeSize, glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));

		// Top Face
		indices->emplace_back(0);
		indices->emplace_back(1);
		indices->emplace_back(2);
		indices->emplace_back(0);
		indices->emplace_back(2);
		indices->emplace_back(3);
		// Bottom Face
		indices->emplace_back(4);
		indices->emplace_back(7);
		indices->emplace_back(6);
		indices->emplace_back(4);
		indices->emplace_back(6);
		indices->emplace_back(5);
		// Left Face
		indices->emplace_back(1);
		indices->emplace_back(0);
		indices->emplace_back(4);
		indices->emplace_back(1);
		indices->emplace_back(4);
		indices->emplace_back(5);
		// Right Face
		indices->emplace_back(2);
		indices->emplace_back(6);
		indices->emplace_back(7);
		indices->emplace_back(2);
		indices->emplace_back(7);
		indices->emplace_back(3);
		// Back Face
		indices->emplace_back(0);
		indices->emplace_back(3);
		indices->emplace_back(4);
		indices->emplace_back(3);
		indices->emplace_back(7);
		indices->emplace_back(4);
		// Front Face
		indices->emplace_back(1);
		indices->emplace_back(5);
		indices->emplace_back(6);
		indices->emplace_back(1);
		indices->emplace_back(6);
		indices->emplace_back(2);
	}




}