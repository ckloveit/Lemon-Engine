#pragma once
#include "Core/Core.h"
#include "RHIDeviceAdapter.h"
#include "RHI.h"
#include "glm/glm.hpp"

namespace Lemon
{
	
	class LEMON_API DynamicRHI
	{
	public:
		/** Declare a virtual destructor, so the dynamic RHI can be deleted without knowing its type. */
		virtual ~DynamicRHI() {}

		virtual void Init() = 0;

		/** Shutdown the RHI; handle shutdown and resource destruction before the RHI's actual destructor is called (so that all resources of the RHI are still available for shutdown). */
		virtual void Shutdown() = 0;

		//===================Begin RHI Methods ==================//
		virtual Ref<RHISwapChain> RHICreateSwapChain(void* windowHandle, const uint32_t width, const uint32_t height, const ERHIFormat format) = 0;



		//========Just Debug
		virtual void RHIClearRenderTarget(Ref<RHISwapChain> swapChain, glm::vec4 backgroundColor) = 0;

		//===================End RHI Methods ==================//

	public:
		//===================Begin Device Adapter======================//
		void AddDeviceAdapter(const RHIDeviceAdapter& deviceAdapter);
		RHIDeviceAdapter* GetMainDeviceAdapter();
		void SetMainDeviceAdapter(const uint32_t index);
		const std::vector<RHIDeviceAdapter>& GetDeviceAdapters() const { return m_DeviceAdapters; }
		//===================End Device Adapter========================//

	protected:
		std::vector<RHIDeviceAdapter> m_DeviceAdapters;
		uint32_t m_MainDeviceAdapterIndex = 0;

	};

	extern LEMON_API DynamicRHI* g_DynamicRHI;

	/**
	*	Each platform that utilizes dynamic RHIs should implement this function
	*	Called to create the instance of the dynamic RHI.
	*/
	DynamicRHI* PlatformCreateDynamicRHI();

	//===================================RHI Resource Create Helper function==================================//
	FORCEINLINE Ref<RHISwapChain> RHICreateSwapChain(void* windowHandle, const uint32_t width, const uint32_t height, const ERHIFormat format)
	{
		return g_DynamicRHI->RHICreateSwapChain(windowHandle, width, height, format);
	}

	FORCEINLINE void RHIClearRenderTarget(Ref<RHISwapChain> swapChain, glm::vec4 backgroundColor)
	{
		return g_DynamicRHI->RHIClearRenderTarget(swapChain, backgroundColor);
	}



}

