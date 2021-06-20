#include "LemonPCH.h"
#include "GlobalRenderResources.h"
#include "RHI/RHI.h"
#include "VertexDeclarationStruct.h"
#include "Containers/DynamicRHIResourceArray.h"
#include "RHI/DynamicRHI.h"


namespace Lemon
{
	GlobalRenderResources* GlobalRenderResources::s_Instance = nullptr;

	void GlobalRenderResources::Init()
	{
		if (s_Instance)
			return;

		s_Instance = new GlobalRenderResources();

		// Init Vertex Declaration Element List
		VertexDeclarationElementList vertexElements;
		const uint32_t stride = sizeof(StandardMeshVertex);
		vertexElements.push_back(RHIVertexElement(0, STRUCT_OFFSET(StandardMeshVertex, Position), VET_Float3, 0, stride));
		vertexElements.push_back(RHIVertexElement(0, STRUCT_OFFSET(StandardMeshVertex, Color), VET_Float4, 1, stride));
		vertexElements.push_back(RHIVertexElement(0, STRUCT_OFFSET(StandardMeshVertex, Normal), VET_Float3, 2, stride));
		vertexElements.push_back(RHIVertexElement(0, STRUCT_OFFSET(StandardMeshVertex, Tangent), VET_Float3, 3, stride));
		const uint8_t texcoordOffset = STRUCT_OFFSET(StandardMeshVertex, Tangent) + sizeof(glm::vec3);
		const uint8_t texcoordAttributeOffset = 4;
		for (int i = 0; i < MAX_MESH_TEXTURE_COORDS; i++)
		{
			const uint8_t offset = texcoordOffset + sizeof(glm::vec2) * i;
			vertexElements.push_back(RHIVertexElement(0, offset, VET_Float2, texcoordAttributeOffset + i, stride));
		}
		s_Instance->StandardMeshVertexDeclarationElementList = std::move(vertexElements);

		// Init FullScreenVertexBuffer
		std::vector<StandardMeshVertex> vertices;
		RHIResourceCreateInfo vertexCreateInfo;
		TResourceArray<StandardMeshVertex> verts;
		vertices.emplace_back(glm::vec3(-1.0f,  1.0f, 0.5f), glm::vec2(0.0f, 0.0f));
		vertices.emplace_back(glm::vec3(-1.0f, -1.0f, 0.5f), glm::vec2(0.0f, 1.0f));
		vertices.emplace_back(glm::vec3( 1.0f, -1.0f, 0.5f), glm::vec2(1.0f, 1.0f));
		vertices.emplace_back(glm::vec3( 1.0f,  1.0f, 0.5f), glm::vec2(1.0f, 0.0f));

		for (int i = 0; i < vertices.size(); i++)
		{
			verts.EmplaceBack(vertices[i]);
		}
		vertexCreateInfo.ResourceArray = &verts;
		s_Instance->FullScreenVertexBuffer = RHICreateVertexBuffer(sizeof(StandardMeshVertex) * vertices.size(), BUF_Static, vertexCreateInfo);

		//Init FullScreenIndexBuffer
		std::vector<uint32_t> indices;
		// 
		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(0);
		indices.emplace_back(2);
		indices.emplace_back(3);

		TResourceArray<uint32_t> indiceResources;
		for (int i = 0; i < indices.size(); i++)
		{
			indiceResources.EmplaceBack(indices[i]);
		}
		RHIResourceCreateInfo indicesCreateInfo;
		indicesCreateInfo.ResourceArray = &indiceResources;
		s_Instance->FullScreenIndexBuffer = RHICreateIndexBuffer(sizeof(uint32_t) * indices.size(), BUF_Static, indicesCreateInfo);
	}
}