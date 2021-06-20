#pragma once
#include "Core/Core.h"
#include "IComponent.h"
#include <glm/glm.hpp>
#include "RenderCore/Mesh.h"

namespace Lemon
{
	class LEMON_API EnvironmentComponent : public IComponent
	{
	public:
		EnvironmentComponent() {}

		void CreateFromFilePath(const std::vector<std::string>& filePaths);

		Ref<RHITextureCube> GetEnvironmentTexture() const { return m_EnvTextureRHI; }

		Ref<RHITextureCube> GetEnvDiffuseIrradiance() const { return m_EnvDiffuseIrradianceTexture; }

		Ref<RHITextureCube> GetEnvSpecularPrefilter() const { return m_EnvSpecularPrefilterTexture; }

		Ref<RHITexture2D> GetEnvSpecularIntegrateBRDF() const { return m_EnvSpecularIntegrateBRDFTexture; }

		void InitEnvDiffuseIrradianceTexture();
		void InitEnvSpecularPrefilterTexture();
		void InitEnvSpecularIntegrateBRDFTexture();
	public:
		//----------Debug IBL--------------//
		// 0 : EnvCubemap
		// 1 : EnvDiffuseIrradiance

		int bDebugShowIBLType = 0;
	private:
		Ref<RHITextureCube> m_EnvTextureRHI;

		//=====Image-Base-Lighting===============
		// Diffuse Irradiance CubeMap
		Ref<RHITextureCube> m_EnvDiffuseIrradianceTexture;
		// Specular Prefilter CubeMap
		Ref<RHITextureCube> m_EnvSpecularPrefilterTexture;
		// Specular IntegrateBRDF
		Ref<RHITexture2D> m_EnvSpecularIntegrateBRDFTexture;

		//=======================================
	};
}