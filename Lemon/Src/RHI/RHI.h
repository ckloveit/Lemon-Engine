#pragma once
#include "Core/Core.h"

namespace Lemon
{
	extern LEMON_API void RHIInit();
	extern LEMON_API void RHIExit();

	class RHISwapChain;

	///RHI Data Type
	enum ERHIFormat : uint32_t // gets serialized so better be explicit
	{
		// R
		RHI_Format_R8_Unorm,
		RHI_Format_R16_Uint,
		RHI_Format_R16_Float,
		RHI_Format_R32_Uint,
		RHI_Format_R32_Float,
		// RG
		RHI_Format_R8G8_Unorm,
		RHI_Format_R16G16_Float,
		RHI_Format_R32G32_Float,
		// RGB
		RHI_Format_R11G11B10_Float,
		RHI_Format_R32G32B32_Float,
		// RGBA
		RHI_Format_R8G8B8A8_Unorm,
		RHI_Format_R10G10B10A2_Unorm,
		RHI_Format_R16G16B16A16_Snorm,
		RHI_Format_R16G16B16A16_Float,
		RHI_Format_R32G32B32A32_Float,
		// DEPTH
		RHI_Format_D32_Float,
		RHI_Format_D32_Float_S8X24_Uint,

		RHI_Format_Undefined
	};
}