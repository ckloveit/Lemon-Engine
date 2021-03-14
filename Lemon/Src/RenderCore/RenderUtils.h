#pragma once
#include "Core/Core.h"
#include <string>
#include "RHI/RHI.h"
#include "RHI/DynamicRHI.h"
#include "RHI/RHIResources.h"

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

	template<typename TBufferStruct>
    class RHIUniformBuffer
	{
	public:
		RHIUniformBuffer()
		{}
		/** Creates a uniform buffer with the given value, and returns a structured reference to it. */
		static Ref<RHIUniformBuffer<TBufferStruct>> CreateUniformBufferImmediate(const std::string& uniformBufferName)
		{
			return CreateRef<RHIUniformBuffer<TBufferStruct>>(RHICreateUniformBuffer<TBufferStruct>(uniformBufferName));
		}
		
		bool UpdateUniformBufferImmediate(const TBufferStruct& value)
		{
			void* data = m_UniformBuffer->Lock();
			memcpy(data, &value, sizeof(TBufferStruct));
			return m_UniformBuffer->UnLock();
		}

		RHIUniformBuffer(RHIUniformBufferBaseRef uniformBuffer)
            : m_UniformBuffer(uniformBuffer)
		{}

		const RHIUniformBufferBaseRef& UniformBuffer() const { return m_UniformBuffer;}
	private:
		RHIUniformBufferBaseRef m_UniformBuffer;
	};
}
