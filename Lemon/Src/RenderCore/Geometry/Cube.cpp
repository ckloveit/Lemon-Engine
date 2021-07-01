#include "LemonPCH.h"
#include "Cube.h"
#include <vector>

namespace Lemon
{
	Cube::Cube(float cubeSize /*= 1.0f*/, bool bCompileDefaultShader /* = true */)
		:m_CubeSize(cubeSize)
	{
		std::vector<StandardMeshVertex> vertices;
		std::vector<uint32_t> indices;
		BuildCube(vertices, indices);
		BuileMesh(vertices, indices);
		// Shader And RHIResouce
		if (bCompileDefaultShader)
		{
			CreateShader<SF_Vertex>("Assets/Shaders/SimpleStandardVertex.hlsl", "MainVS");
			CreateShader<SF_Pixel>("Assets/Shaders/SimpleStandardPixel.hlsl", "MainPS");
			CreateRHIBuffers();
		}
	}

	void Cube::BuildCube(std::vector<StandardMeshVertex>& vertices, std::vector<uint32_t>& indices)
	{
		/*
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
		*/

		const float length = 1.0f;
		const float width = 1.0f;
		const float height = 1.0f;

		glm::vec3 p0 = glm::vec3(-length * .5f, -width * .5f, height * .5f);
		glm::vec3 p1 = glm::vec3(length * .5f, -width * .5f, height * .5f);
		glm::vec3 p2 = glm::vec3(length * .5f, -width * .5f, -height * .5f);
		glm::vec3 p3 = glm::vec3(-length * .5f, -width * .5f, -height * .5f);

		glm::vec3 p4 = glm::vec3(-length * .5f, width * .5f, height * .5f);
		glm::vec3 p5 = glm::vec3(length * .5f, width * .5f, height * .5f);
		glm::vec3 p6 = glm::vec3(length * .5f, width * .5f, -height * .5f);
		glm::vec3 p7 = glm::vec3(-length * .5f, width * .5f, -height * .5f);

		vertices.resize(24);

		vertices[0].Position = p0;
		vertices[1].Position = p1;
		vertices[2].Position = p2;
		vertices[3].Position = p3;

		vertices[4].Position = p7;
		vertices[5].Position = p4;
		vertices[6].Position = p0;
		vertices[7].Position = p3;

		vertices[8].Position = p4;
		vertices[9].Position = p5;
		vertices[10].Position = p1;
		vertices[11].Position = p0;

		vertices[12].Position = p6;
		vertices[13].Position = p7;
		vertices[14].Position = p3;
		vertices[15].Position = p2;

		vertices[16].Position = p5;
		vertices[17].Position = p6;
		vertices[18].Position = p2;
		vertices[19].Position = p1;

		vertices[20].Position = p7;
		vertices[21].Position = p6;
		vertices[22].Position = p5;
		vertices[23].Position = p4;

		indices.resize(36);
		int index = 0;
		for (int i = 0; i < 6; ++i)
		{
			indices[index++] = i * 4;
			indices[index++] = i * 4 + 1;
			indices[index++] = i * 4 + 3;

			indices[index++] = i * 4 + 1;
			indices[index++] = i * 4 + 2;
			indices[index++] = i * 4 + 3;
		}
	}

}