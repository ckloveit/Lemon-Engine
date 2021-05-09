#pragma once
#include "Core/Core.h"
#include "RHI/RHICommandList.h"

namespace Lemon
{
	class RHIVertexShader;
	class RHIPixelShader;
	class RHIVertexDeclaration;

	struct PixelShaderUtils
	{
		static void DrawFullScreenQuad(Ref<RHICommandList>& RHICmdList,
			std::shared_ptr<RHIVertexDeclaration> VertexDeclaration,
			std::shared_ptr<RHIVertexShader> VertexShader,
			std::shared_ptr<RHIPixelShader> PixelShader,
			std::vector<std::shared_ptr<RHITexture>> Textures = std::vector<std::shared_ptr<RHITexture>>());

	};
}
