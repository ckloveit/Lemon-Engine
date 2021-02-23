#pragma once
#include "Core/Core.h"

namespace Lemon
{
	extern LEMON_API void RHIInit();
	extern LEMON_API void RHIExit();

	class RHISwapChain;

	struct RHIResourceCreateInfo
	{
		std::vector<std::vector<std::byte>> RawDatas;


	};

	///RHI Data Type
	enum ERHIPixelFormat : uint32_t // gets serialized so better be explicit
	{
		// R
		RHI_PF_R8_Unorm,
		RHI_PF_R16_Uint,
		RHI_PF_R16_Float,
		RHI_PF_R32_Uint,
		RHI_PF_R32_Float,
		// RG
		RHI_PF_R8G8_Unorm,
		RHI_PF_R16G16_Float,
		RHI_PF_R32G32_Float,
		// RGB
		RHI_PF_R11G11B10_Float,
		RHI_PF_R32G32B32_Float,
		// RGBA
		RHI_PF_R8G8B8A8_Unorm,
		RHI_PF_R10G10B10A2_Unorm,
		RHI_PF_R16G16B16A16_Snorm,
		RHI_PF_R16G16B16A16_Float,
		RHI_PF_R32G32B32A32_Float,
		// DEPTH
		RHI_PF_D32_Float,
		RHI_PF_D32_Float_S8X24_Uint,
		// UNKNOWN
		RHI_PF_Undefined,
		//
		RHI_PF_MAX
	};

	enum ERHITextureCreateFlags
	{
		RHI_TexCreate_None = 0,
		// Texture can be used as a render target
		RHI_TexCreate_RenderTargetable = 1 << 0,
		// Texture can be used as a depth-stencil target.
		RHI_TexCreate_DepthStencilTargetable = 1 << 1,
		// Texture can be used as a shader resource.
		RHI_TexCreate_ShaderResource = 1 << 2,
		// UnorderedAccessView (DX11 only)
		RHI_TexCreate_UnorderedAccessView = 1 << 3,


	};


}