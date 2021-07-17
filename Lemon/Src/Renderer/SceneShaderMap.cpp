#include "LemonPCH.h"
#include "SceneShaderMap.h"

#include "RHI/RHI.h"
#include "RHI/DynamicRHI.h"
#include "RHI/RHIResources.h"
#include "RenderCore/VertexDeclarationStruct.h"
#include "RenderCore/GlobalRenderResources.h"
namespace Lemon
{
	void SceneShaderMap::Allocate()
	{
		RHIShaderCreateInfo shaderCreateInfo;

		// -------------------Depth Only--------------------------------------//
		m_DepthOnlyVS = RHICreateVertexShader("Assets/Shaders/DepthOnlyVertex.hlsl", "MainVS", shaderCreateInfo);
		m_DepthOnlyPS = RHICreatePixelShader("Assets/Shaders/DepthOnlyPixel.hlsl", "MainPS", shaderCreateInfo);
		
		/*VertexDeclarationElementList vertexElements;
		const uint32_t stride = sizeof(DepthOnlyVertex);
		vertexElements.push_back(RHIVertexElement(0, STRUCT_OFFSET(DepthOnlyVertex, Position), VET_Float3, 0, stride));
		m_DepthOnlyDeclaration = RHICreateVertexDeclaration(m_DepthOnlyVS, vertexElements);
		*/
		if (GlobalRenderResources::GetInstance())
		{
			m_DepthOnlyDeclaration = RHICreateVertexDeclaration(m_DepthOnlyVS, GlobalRenderResources::GetInstance()->StandardMeshVertexDeclarationElementList);
		}

		//------------------GBuffer Geometry Gather Pass----------------------------------//
		m_GBufferGeometryVS = RHICreateVertexShader("Assets/Shaders/GBufferGeometryVertex.hlsl", "MainVS", shaderCreateInfo);
		m_GBufferGeometryPS = RHICreatePixelShader("Assets/Shaders/GBufferGeometryPixel.hlsl", "MainPS", shaderCreateInfo);
		if (GlobalRenderResources::GetInstance())
		{
			m_GBufferGeometryDeclaration = RHICreateVertexDeclaration(m_GBufferGeometryVS, GlobalRenderResources::GetInstance()->StandardMeshVertexDeclarationElementList);
		}

		// GBuffer for Sky
		m_GBufferGeometrySkyVS = RHICreateVertexShader("Assets/Shaders/GBufferGeometrySkyVertex.hlsl", "MainVS", shaderCreateInfo);
		if (GlobalRenderResources::GetInstance())
		{
			m_GBufferGeometrySkyDeclaration = RHICreateVertexDeclaration(m_GBufferGeometrySkyVS, GlobalRenderResources::GetInstance()->StandardMeshVertexDeclarationElementList);
		}
		m_GBufferGeometrySkyPS = RHICreatePixelShader("Assets/Shaders/GBufferGeometrySkyPixel.hlsl", "MainPS", shaderCreateInfo);

		//------------------GBuffer Lighting Pass----------------------------------//
		m_GBufferLightingVS = RHICreateVertexShader("Assets/Shaders/GBufferLightingVertex.hlsl", "MainVS", shaderCreateInfo);
		m_GBufferLightingPS = RHICreatePixelShader("Assets/Shaders/GBufferLightingPixel.hlsl", "MainPS", shaderCreateInfo);
		if (GlobalRenderResources::GetInstance())
		{
			m_GBufferLightingDeclaration = RHICreateVertexDeclaration(m_GBufferLightingVS, GlobalRenderResources::GetInstance()->StandardMeshVertexDeclarationElementList);
		}

		//m_GBufferLightingDeclaration
	}

}