#pragma once
#include "Core/Core.h"
#include "Log/Log.h"
#include <dxgi1_6.h>
#include "RHI/DynamicRHI.h"
#include "RHI/RHIDeviceAdapter.h"
#include "RHI/RHI.h"
#include "RHI/D3D11/D3D11RHI.h"

namespace Lemon::D3D11
{
	template <typename T>
	constexpr void SafeRelease(T*& ptr)
	{
		if (ptr)
		{
			ptr->Release();
			ptr = nullptr;
		}
	}

	inline const char* DXGIErrorToString(const HRESULT errorCode)
	{
		switch (errorCode)
		{
		case DXGI_ERROR_DEVICE_HUNG:                    return "DXGI_ERROR_DEVICE_HUNG";                    // The application's device failed due to badly formed commands sent by the application. This is an design-time issue that should be investigated and fixed.
		case DXGI_ERROR_DEVICE_REMOVED:                 return "DXGI_ERROR_DEVICE_REMOVED";                 // The video card has been physically removed from the system, or a driver upgrade for the video card has occurred. The application should destroy and recreate the device. For help debugging the problem, call ID3D10Device::GetDeviceRemovedReason.
		case DXGI_ERROR_DEVICE_RESET:                   return "DXGI_ERROR_DEVICE_RESET";                   // The device failed due to a badly formed command. This is a run-time issue; The application should destroy and recreate the device.
		case DXGI_ERROR_DRIVER_INTERNAL_ERROR:          return "DXGI_ERROR_DRIVER_INTERNAL_ERROR";          // The driver encountered a problem and was put into the device removed state.
		case DXGI_ERROR_FRAME_STATISTICS_DISJOINT:      return "DXGI_ERROR_FRAME_STATISTICS_DISJOINT";      // An event (for example, a power cycle) interrupted the gathering of presentation statistics.
		case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE:   return "DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE";   // The application attempted to acquire exclusive ownership of an output, but failed because some other application (or device within the application) already acquired ownership.
		case DXGI_ERROR_INVALID_CALL:                   return "DXGI_ERROR_INVALID_CALL";                   // The application provided invalid parameter data; this must be debugged and fixed before the application is released.
		case DXGI_ERROR_MORE_DATA:                      return "DXGI_ERROR_MORE_DATA";                      // The buffer supplied by the application is not big enough to hold the requested data.
		case DXGI_ERROR_NONEXCLUSIVE:                   return "DXGI_ERROR_NONEXCLUSIVE";                   // A global counter resource is in use, and the Direct3D device can't currently use the counter resource.
		case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:        return "DXGI_ERROR_NOT_CURRENTLY_AVAILABLE";        // The resource or request is not currently available, but it might become available later.
		case DXGI_ERROR_NOT_FOUND:                      return "DXGI_ERROR_NOT_FOUND";                      // When calling IDXGIObject::GetPrivateData, the GUID passed in is not recognized as one previously passed to IDXGIObject::SetPrivateData or IDXGIObject::SetPrivateDataInterface. When calling IDXGIFentityy::EnumAdapters or IDXGIAdapter::EnumOutputs, the enumerated ordinal is out of range.
		case DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED:     return "DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED";     // Reserved
		case DXGI_ERROR_REMOTE_OUTOFMEMORY:             return "DXGI_ERROR_REMOTE_OUTOFMEMORY";             // Reserved
		case DXGI_ERROR_WAS_STILL_DRAWING:              return "DXGI_ERROR_WAS_STILL_DRAWING";              // The GPU was busy at the moment when a call was made to perform an operation, and did not execute or schedule the operation.
		case DXGI_ERROR_UNSUPPORTED:                    return "DXGI_ERROR_UNSUPPORTED";                    // The requested functionality is not supported by the device or the driver.
		case DXGI_ERROR_ACCESS_LOST:                    return "DXGI_ERROR_ACCESS_LOST";                    // The desktop duplication interface is invalid. The desktop duplication interface typically becomes invalid when a different type of image is displayed on the desktop.
		case DXGI_ERROR_WAIT_TIMEOUT:                   return "DXGI_ERROR_WAIT_TIMEOUT";                   // The time-out interval elapsed before the next desktop frame was available.
		case DXGI_ERROR_SESSION_DISCONNECTED:           return "DXGI_ERROR_SESSION_DISCONNECTED";           // The Remote Desktop Services session is currently disconnected.
		case DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE:       return "DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE";       // The DXGI output (monitor) to which the swap chain content was restricted is now disconnected or changed.
		case DXGI_ERROR_CANNOT_PROTECT_CONTENT:         return "DXGI_ERROR_CANNOT_PROTECT_CONTENT";         // DXGI can't provide content protection on the swap chain. This error is typically caused by an older driver, or when you use a swap chain that is incompatible with content protection.
		case DXGI_ERROR_ACCESS_DENIED:                  return "DXGI_ERROR_ACCESS_DENIED";                  // You tried to use a resource to which you did not have the required access privileges. This error is most typically caused when you write to a shared resource with read-only access.
		case DXGI_ERROR_NAME_ALREADY_EXISTS:            return "DXGI_ERROR_NAME_ALREADY_EXISTS";            // The supplied name of a resource in a call to IDXGIResource1::CreateSharedHandle is already associated with some other resource.
		case DXGI_ERROR_SDK_COMPONENT_MISSING:          return "DXGI_ERROR_SDK_COMPONENT_MISSING";          // The application requested an operation that depends on an SDK component that is missing or mismatched.
		case DXGI_ERROR_NOT_CURRENT:                    return "DXGI_ERROR_NOT_CURRENT";                    // The DXGI objects that the application has created are no longer current & need to be recreated for this operation to be performed.
		case DXGI_ERROR_HW_PROTECTION_OUTOFMEMORY:      return "DXGI_ERROR_HW_PROTECTION_OUTOFMEMORY";      // Insufficient HW protected memory exits for proper function.
		case DXGI_ERROR_DYNAMIC_CODE_POLICY_VIOLATION:  return "DXGI_ERROR_DYNAMIC_CODE_POLICY_VIOLATION";  // Creating this device would violate the process's dynamic code policy.
		case DXGI_ERROR_NON_COMPOSITED_UI:              return "DXGI_ERROR_NON_COMPOSITED_UI";              // The operation failed because the compositor is not in control of the output.
		case E_INVALIDARG:                              return "E_INVALIDARG";                              // One or more arguments are invalid.
		}

		return "Unknown error code";
	}

	constexpr bool DXGIErrorCheck(const HRESULT result)
	{
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("%s", DXGIErrorToString(result));
			return false;
		}
		return true;
	}

	inline void DetectAdapters()
	{
		// Create DirectX graphics interface factory
		IDXGIFactory1* factory;
		const auto result = CreateDXGIFactory1(IID_PPV_ARGS(&factory));

		if (FAILED(result))
		{
			LEMON_CORE_ERROR("Failed to create a DirectX graphics interface factory");// {0}.", dxgi_error_to_string(result));
			return;
		}

		const auto GetAvailableAdapters = [](IDXGIFactory1* factory)
		{
			uint32_t i = 0;
			IDXGIAdapter* adapter;
			std::vector<IDXGIAdapter*> adapters;
			while (factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
			{
				adapters.emplace_back(adapter);
				++i;
			}

			return adapters;
		};
		// Get all available adapters
		auto adapters = GetAvailableAdapters(factory);
		if (factory)
		{
			factory->Release();
			factory = nullptr;
		}

		if (adapters.empty())
		{
			LEMON_CORE_ERROR("Couldn't find any adapters");
			return;
		}

		// Save all available adapters
		DXGI_ADAPTER_DESC adapterDesc;
		for (auto displayAdapter : adapters)
		{
			if (FAILED(displayAdapter->GetDesc(&adapterDesc)))
			{
				LEMON_CORE_ERROR("Failed to get adapter description");
				continue;
			}

			// Of course it wouldn't be simple, lets convert the device name
			char name[128];
			auto def_char = ' ';
			WideCharToMultiByte(CP_ACP, 0, adapterDesc.Description, -1, name, 128, &def_char, nullptr);

			if (g_DynamicRHI)
			{
				g_DynamicRHI->AddDeviceAdapter(RHIDeviceAdapter
				(
					adapterDesc.VendorId,										 // vendor id
					&name[0],                                                   // name
					static_cast<uint32_t>(adapterDesc.DedicatedVideoMemory),   // memory
					static_cast<void*>(displayAdapter))                        // data
				);
			}
		}
		// DISPLAY MODES
		const auto GetDisplayModes = [](IDXGIAdapter* adapter, ERHIFormat format)
		{
			// Enumerate the primary adapter output (monitor).
			IDXGIOutput* adapterOutput = nullptr;
			bool result = SUCCEEDED(adapter->EnumOutputs(0, &adapterOutput));
			if (result)
			{
				// Get supported display mode count
				UINT displayModeCount = 0;
				result = SUCCEEDED(adapterOutput->GetDisplayModeList(D3D11Format[format], DXGI_ENUM_MODES_INTERLACED, &displayModeCount, nullptr));
				if (result)
				{
					// Get display modes
					std::vector<DXGI_MODE_DESC> displayModes;
					displayModes.resize(displayModeCount);
					result = SUCCEEDED(adapterOutput->GetDisplayModeList(D3D11Format[format], DXGI_ENUM_MODES_INTERLACED, &displayModeCount, &displayModes[0]));
					
				}
				adapterOutput->Release();
			}

			return result;
		};

		// Get display modes and set primary adapter
		for (uint32_t deviceIndex = 0; deviceIndex < (uint32_t)g_DynamicRHI->GetDeviceAdapters().size(); deviceIndex++)
		{
			const RHIDeviceAdapter& deviceAdapter = g_DynamicRHI->GetDeviceAdapters()[deviceIndex];
			const auto dx_adapter = static_cast<IDXGIAdapter*>(deviceAdapter.GetAdapter());

			// Adapters are ordered by memory (descending), so stop on the first success
			const auto format = RHI_Format_R8G8B8A8_Unorm; // TODO: This must come from the swapchain
			if (GetDisplayModes(dx_adapter, format))
			{
				g_DynamicRHI->SetMainDeviceAdapter(deviceIndex);
				return;
			}
			else
			{
				LEMON_CORE_ERROR("Failed to get display modes for \"{0}\".", deviceAdapter.GetName().c_str());
			}
		}

	}

}