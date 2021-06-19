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

		Ref<RHITextureCube> GetEnvironmentTexture() const { return m_EnvironmentTextureRHI; }

		Ref<RHITextureCube> GetEnvDiffuseIrradiance() const { return m_EnvironmentDiffuseIrradianceTexture; }

		Ref<RHITextureCube> GetEnvSpecularPrefilter() const { return m_EnvironmentSpecularPrefilterTexture; }

		void InitEnvDiffuseIrradianceTexture();
		void InitEnvSpecularPrefilterTexture();
	public:
		//----------Debug IBL--------------//
		// 0 : EnvCubemap
		// 1 : EnvDiffuseIrradiance

		int bDebugShowIBLType = 0;
	private:
		Ref<RHITextureCube> m_EnvironmentTextureRHI;

		//=====Image-Base-Lighting===============
		// Diffuse Irradiance CubeMap
		Ref<RHITextureCube> m_EnvironmentDiffuseIrradianceTexture;
		// Specular Prefilter CubeMap
		Ref<RHITextureCube> m_EnvironmentSpecularPrefilterTexture;
		

		//=======================================
	};
}