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

		void InitEnvDiffuseIrradianceTexture();

		Ref<RHITextureCube> GetEnvDiffuseIrradiance() const { return m_EnvironmentDiffuseIrradianceTexture; }
	private:
		Ref<RHITextureCube> m_EnvironmentTextureRHI;

		//Image-Base-Lighting
		Ref<RHITextureCube> m_EnvironmentDiffuseIrradianceTexture;
	};
}