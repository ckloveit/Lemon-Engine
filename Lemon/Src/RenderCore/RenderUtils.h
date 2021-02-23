#pragma once
#include "Core/Core.h"
#include <string>
#include "RHI/RHI.h"

namespace Lemon
{
	/** Information about a pixel format. */
	struct PixelFormatInfo
	{
		std::string Name;
		int32_t			NumBytes;
		int32_t			NumComponents;
		/** Platform specific token, e.g. D3DFORMAT with D3DDrv										*/
		int32_t			PlatformFormat;
		/** Whether the texture format is supported on the current platform/ rendering combination	*/
		bool			Supported;
		ERHIPixelFormat	RHIFormat;
	};

	extern LEMON_API PixelFormatInfo GPixelFormats[RHI_PF_MAX];		// Maps members of EPixelFormat to a FPixelFormatInfo describing the format.

}
