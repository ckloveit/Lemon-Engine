#include "LemonPCH.h"
#include "RenderUtils.h"

namespace Lemon
{
	PixelFormatInfo	GPixelFormats[RHI_PF_MAX] =
	{
		{ "RHI_PF_R8_Unorm",				 1,		1,		0,	1,	RHI_PF_R8_Unorm},
		{ "RHI_PF_R16_Uint",				 2,		1,		0,	1,	RHI_PF_R16_Uint},
		{ "RHI_PF_R16_Float",				 2,		1,		0,	1,	RHI_PF_R16_Float},
		{ "RHI_PF_R32_Uint",				 4,		1,		0,	1,	RHI_PF_R32_Uint},
		{ "RHI_PF_R32_Float",				 4,		1,		0,	1,	RHI_PF_R32_Float},
		{ "RHI_PF_R8G8_Unorm",				 2,		2,		0,	1,	RHI_PF_R8G8_Unorm},
		{ "RHI_PF_R16G16_Float",			 4,		2,		0,	1,	RHI_PF_R16G16_Float},
		{ "RHI_PF_R32G32_Float",			 8,		2,		0,	1,	RHI_PF_R32G32_Float},
		{ "RHI_PF_R11G11B10_Float",			 4,		3,		0,	1,	RHI_PF_R11G11B10_Float},
		{ "RHI_PF_R32G32B32_Float",			12,		3,		0,	1,	RHI_PF_R32G32B32_Float},
		{ "RHI_PF_R8G8B8A8_Unorm",			 4,		4,		0,	1,	RHI_PF_R8G8B8A8_Unorm},
		{ "RHI_PF_R10G10B10A2_Unorm",		 4,		4,		0,	1,	RHI_PF_R10G10B10A2_Unorm},
		{ "RHI_PF_R16G16B16A16_Snorm",		 8,		4,		0,	1,	RHI_PF_R16G16B16A16_Snorm},
		{ "RHI_PF_R16G16B16A16_Float",		 8,		4,		0,	1,	RHI_PF_R16G16B16A16_Float},
		{ "RHI_PF_R32G32B32A32_Float",		16,		4,		0,	1,	RHI_PF_R32G32B32A32_Float},
		{ "RHI_PF_D32_Float",				 4,		1,		0,	1,	RHI_PF_D32_Float},
		{ "RHI_PF_D32_Float_S8X24_Uint",	 8,		1,		0,	1,	RHI_PF_D32_Float_S8X24_Uint},
		{ "RHI_PF_Undefined",				 0,		0,		0,	1,	RHI_PF_Undefined},

	};


}