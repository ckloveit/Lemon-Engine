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

		Ref<RHITexture> GetEnvironmentTexture() const { return m_EnvironmentTextureRHI; }
	private:
		Ref<RHITexture> m_EnvironmentTextureRHI;
	};
}