#include "LemonPCH.h"
#include "DynamicRHI.h"
#include "Log/Log.h"
#ifdef LEMON_GRAPHICS_D3D11
#include "D3D11/D3D11DynamicRHI.h"
#elif LEMON_GRAPHICS_D3D12

#endif

namespace Lemon
{
	// Globals.
	DynamicRHI* g_DynamicRHI = nullptr;

	void RHIInit()
	{
		if (!g_DynamicRHI)
		{
			g_DynamicRHI = PlatformCreateDynamicRHI();
			if (g_DynamicRHI)
			{
				g_DynamicRHI->Init();
			}
		}
	}

	void RHIExit()
	{
		if (!g_DynamicRHI)
		{
			g_DynamicRHI->Shutdown();
		}

		delete g_DynamicRHI;
		g_DynamicRHI = nullptr;
	}

	DynamicRHI* PlatformCreateDynamicRHI()
	{
#ifdef LEMON_GRAPHICS_D3D11
		return new D3D11DynamicRHI();
#endif
		return nullptr;
	}

	//----------------------------------------------------------------------------------//
	void DynamicRHI::AddDeviceAdapter(const RHIDeviceAdapter& deviceAdapter)
	{
		m_DeviceAdapters.emplace_back(deviceAdapter);

		// Keep devices sorted, based on memory (from highest to lowest)
		std::sort(m_DeviceAdapters.begin(), m_DeviceAdapters.end(), [](const RHIDeviceAdapter& adapter1, const RHIDeviceAdapter& adapter2)
			{
				return adapter1.GetMemory() > adapter2.GetMemory();
			});

		LEMON_CORE_INFO("{0} ({1} MB)", deviceAdapter.GetName().c_str(), deviceAdapter.GetMemory());
	}

	RHIDeviceAdapter* DynamicRHI::GetMainDeviceAdapter()
	{
		if (m_MainDeviceAdapterIndex >= m_DeviceAdapters.size())
			return nullptr;

		return &m_DeviceAdapters[m_MainDeviceAdapterIndex];
	}

	void DynamicRHI::SetMainDeviceAdapter(const uint32_t index)
	{
		m_MainDeviceAdapterIndex = index;

		if (const RHIDeviceAdapter * deviceAdapter = GetMainDeviceAdapter())
		{
			LEMON_CORE_INFO("{0} ({1} MB)", deviceAdapter->GetName().c_str(), deviceAdapter->GetMemory());
		}
	}
	//----------------------------------------------------------------------------------//


}


