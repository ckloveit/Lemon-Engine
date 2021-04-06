#pragma once
#include "Core/Core.h"
#include <vector>
#include <glm/glm.hpp>
#include "RHI/RHI.h"
#include "RHI/DynamicRHI.h"
#include "RHI/RHIResources.h"

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
			, Normal(glm::vec3(0,0,1))
			, Tangent(glm::vec3(1,0,0))
		{
			for(int i = 0;i < MAX_MESH_TEXTURE_COORDS; i++)
				Texcoords[i] = glm::vec2(0,0);
		}
	};

	class LEMON_API Mesh
	{
	public:
		Mesh() = default;
		virtual  ~Mesh() {}

		//==Mesh Utilities
		void BuileMesh(const std::vector<StandardMeshVertex>& vertices, const std::vector<uint32_t>& indices);

		template<EShaderFrequency ShaderType>
		void CreateShader(const std::string& shaderPath, const std::string& entryPoint)
		{
			RHIShaderCreateInfo shaderCreateInfo;
			if(ShaderType == EShaderFrequency::SF_Vertex)
			{
				m_VertexShader = RHICreateVertexShader(shaderPath, entryPoint, shaderCreateInfo);
			}
			else if(ShaderType == EShaderFrequency::SF_Pixel)
			{
				m_PixelShader = RHICreatePixelShader(shaderPath, entryPoint, shaderCreateInfo);
			}
		}
		
		void CreateRHIBuffers();

		//=== Draw Data Getter
		const std::shared_ptr<RHIVertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
		const std::shared_ptr<RHIIndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
		const std::shared_ptr<RHIVertexShader>& GetVertexShader() const { return m_VertexShader; }
		const std::shared_ptr<RHIPixelShader>& GetPixelShader() const { return m_PixelShader; }
		const std::shared_ptr<RHIVertexDeclaration>& GetVertexDeclaration() const { return  m_VertexDeclaration; }
		uint32_t GetIndexCount() const {return (uint32_t)m_Indices.size(); }

		//=== Render Stage====//
		const Ref<RHIRasterizerState>& GetRasterizerState() const { return m_RasterizerState; }
		void SetRasterizerState(Ref<RHIRasterizerState>& state) { m_RasterizerState = state; }

		
		const Ref<RHIBlendState>& GetBlendState() const { return m_BlendState; }
		void SetBlendState(Ref<RHIBlendState>& state) { m_BlendState = state; }

		const Ref<RHIDepthStencilState>& GetDepthStencilState() const { return m_DepthStencilState; }
		void SetDepthStencilState(Ref<RHIDepthStencilState>& state) { m_DepthStencilState = state; }

		const EPrimitiveType GetPrimitiveType() const { return m_PrimitiveType; }
		void SetPrimitiveType(EPrimitiveType primitiveType) { m_PrimitiveType = primitiveType; }
	protected:
		std::vector<StandardMeshVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		// Render State
		Ref<RHIBlendState> m_BlendState = nullptr;
		Ref<RHIRasterizerState> m_RasterizerState = nullptr;
		Ref<RHIDepthStencilState> m_DepthStencilState = nullptr;
		EPrimitiveType m_PrimitiveType = EPrimitiveType::PT_TriangleList;
				
		// Draw Data
		std::shared_ptr<RHIVertexBuffer> m_VertexBuffer = nullptr;
		std::shared_ptr<RHIIndexBuffer> m_IndexBuffer = nullptr;
		std::shared_ptr<RHIVertexDeclaration> m_VertexDeclaration = nullptr;
		std::shared_ptr<RHIVertexShader> m_VertexShader = nullptr;
		std::shared_ptr<RHIPixelShader> m_PixelShader = nullptr;
	};
}
