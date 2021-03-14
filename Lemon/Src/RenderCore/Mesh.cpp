#include "LemonPCH.h"
#include "Mesh.h"
#include "Containers/DynamicRHIResourceArray.h"

namespace Lemon
{
	void Mesh::BuileMesh(const std::vector<StandardMeshVertex>& vertices, const std::vector<uint32_t>& indices)
	{
		m_Vertices = vertices;
		m_Indices = indices;
	}

	void Mesh::CreateRHIBuffers()
	{
		Check(m_VertexShader && m_PixelShader);
		RHIResourceCreateInfo vertexCreateInfo;
		TResourceArray<StandardMeshVertex> verts;
		for(int i = 0;i < m_Vertices.size(); i++)
		{
			verts.EmplaceBack(m_Vertices[i]);
		}
		vertexCreateInfo.ResourceArray = &verts;
		m_VertexBuffer = RHICreateVertexBuffer(sizeof(StandardMeshVertex) * m_Vertices.size(), BUF_Static, vertexCreateInfo);

		TResourceArray<uint32_t> indices;
		for(int i =0; i < m_Indices.size(); i++)
		{
			indices.EmplaceBack(m_Indices[i]);
		}
		RHIResourceCreateInfo indicesCreateInfo;
		indicesCreateInfo.ResourceArray = &indices;
		m_IndexBuffer = RHICreateIndexBuffer(sizeof(uint32_t) * m_Indices.size(), BUF_Static, indicesCreateInfo);

		VertexDeclarationElementList vertexElements;
		const uint32_t stride = sizeof(StandardMeshVertex);
		vertexElements.push_back(RHIVertexElement(0, STRUCT_OFFSET(StandardMeshVertex, Position), VET_Float3, 0, stride));
		vertexElements.push_back(RHIVertexElement(0, STRUCT_OFFSET(StandardMeshVertex, Color), VET_Float4, 1, stride));
		vertexElements.push_back(RHIVertexElement(0, STRUCT_OFFSET(StandardMeshVertex, Normal), VET_Float3, 2, stride));
		vertexElements.push_back(RHIVertexElement(0, STRUCT_OFFSET(StandardMeshVertex, Tangent), VET_Float3, 3, stride));
		const uint8_t texcoordOffset = STRUCT_OFFSET(StandardMeshVertex, Tangent) + sizeof(glm::vec3);
		const uint8_t texcoordAttributeOffset = 4;
		for(int i = 0;i < MAX_MESH_TEXTURE_COORDS; i++)
		{
			const uint8_t offset = texcoordOffset + sizeof(glm::vec2) * i;
			vertexElements.push_back(RHIVertexElement(0, offset, VET_Float2, texcoordAttributeOffset + i, stride));
		}
		m_VertexDeclaration = RHICreateVertexDeclaration(m_VertexShader, vertexElements);
	}
	
}