#pragma once
#include "Core/TSingleon.h"

namespace Lemon
{
	class RHIVertexDeclaration;
	class RHIVertexShader;
	class RHIPixelShader;

	class LEMON_API SceneShaderMap : public TSingleton<SceneShaderMap>
	{
	public:
		SceneShaderMap()
			: Super()
		{

		}
		~SceneShaderMap() {}

		void Allocate();

	public:

		// ----------Depth--------------//
		Ref<RHIVertexDeclaration> m_DepthOnlyDeclaration = nullptr;
		Ref<RHIVertexShader> m_DepthOnlyVS = nullptr;
		Ref<RHIPixelShader> m_DepthOnlyPS = nullptr;

		// ---------GBuffer Geometry state------------//
		Ref<RHIVertexDeclaration> m_GBufferGeometryDeclaration = nullptr;
		Ref<RHIVertexShader> m_GBufferGeometryVS = nullptr;
		Ref<RHIPixelShader> m_GBufferGeometryPS = nullptr;

		// Special for SkyBox
		Ref<RHIVertexDeclaration> m_GBufferGeometrySkyDeclaration = nullptr;
		Ref<RHIVertexShader> m_GBufferGeometrySkyVS = nullptr;
		Ref<RHIPixelShader> m_GBufferGeometrySkyPS = nullptr;


		// --------GBuffer Lighting state--------//
		Ref<RHIVertexDeclaration> m_GBufferLightingDeclaration = nullptr;
		Ref<RHIVertexShader> m_GBufferLightingVS = nullptr;
		Ref<RHIPixelShader> m_GBufferLightingPS = nullptr;

	};
}