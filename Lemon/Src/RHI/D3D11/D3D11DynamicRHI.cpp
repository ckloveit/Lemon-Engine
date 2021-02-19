#include "LemonPCH.h"
#include "D3D11DynamicRHI.h"
#include "Log/Log.h"
#include <dxgi1_6.h>
#include "RHI/D3D11/D3D11SwapChain.h"
#include <glm/gtc/type_ptr.hpp>

namespace Lemon
{
	void D3D11DynamicRHI::Init()
	{
		InitD3DDevice();
	}

	void D3D11DynamicRHI::Shutdown()
	{

	}

	void D3D11DynamicRHI::InitD3DDevice()
	{
		if (!m_Direct3DDevice)
		{
			LEMON_CORE_INFO("Create new Direct3DDevice");

			//Detect Adapters
			D3D11::DetectAdapters();

			const RHIDeviceAdapter* deviceAdapter = GetMainDeviceAdapter();
			if (!deviceAdapter)
			{
				LEMON_CORE_ERROR("Failed to detect any devices");
				return;
			}

			//Create device

			uint32_t deviceFlags = 0;

			#if LEMON_DEBUG
				deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
			#endif
			// The order of the feature levels that we'll try to create a device with
			std::vector<D3D_FEATURE_LEVEL> featureLevels =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};
			auto adapter = static_cast<IDXGIAdapter*>(deviceAdapter->GetAdapter());
			auto driverType = adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE;

			auto CreateDevice = [this, &adapter, &driverType, &deviceFlags, &featureLevels]()
			{
				ID3D11Device* tempDevice = nullptr;
				ID3D11DeviceContext* tempContext = nullptr;

				const HRESULT result = D3D11CreateDevice(
					adapter,                                    // pAdapter: If nullptr, the default adapter will be used
					driverType,                                 // DriverType
					nullptr,                                    // HMODULE: nullptr because DriverType = D3D_DRIVER_TYPE_HARDWARE
					deviceFlags,                                // Flags
					featureLevels.data(),                        // pFeatureLevels
					static_cast<UINT>(featureLevels.size()),    // FeatureLevels
					D3D11_SDK_VERSION,                            // SDKVersion
					&tempDevice,                                // ppDevice
					nullptr,                                    // pFeatureLevel
					&tempContext                                // ppImmediateContext
				);

				if (SUCCEEDED(result))
				{
					// Query old device for newer interface.
					if (!D3D11::DXGIErrorCheck(tempDevice->QueryInterface(__uuidof(ID3D11Device5), (void**)&m_Direct3DDevice)))
						return E_FAIL;

					// Release old device.
					D3D11::SafeRelease(tempDevice);

					// Query old device context for newer interface.
					if (!D3D11::DXGIErrorCheck(tempContext->QueryInterface(__uuidof(ID3D11DeviceContext4), (void**)&m_Direct3DDeviceIMContext)))
						return E_FAIL;

					// Release old context.
					D3D11::SafeRelease(tempContext);
				}
				return result;
			};

			// Create Direct3D device and Direct3D device context.
			auto result = CreateDevice();

			if (FAILED(result))
			{
				LEMON_CORE_ERROR("Failed to create device, %s.", D3D11::DXGIErrorToString(result));
				return;
			}
		}
	}


	//===================Begin RHI Methods ==================//
	Ref<RHISwapChain> D3D11DynamicRHI::RHICreateSwapChain(void* windowHandle, const uint32_t width, const uint32_t height, const ERHIFormat format)
	{
		return CreateRef<D3D11SwapChain>(this, windowHandle, width, height, format);
	}

	void D3D11DynamicRHI::RHIClearRenderTarget(Ref<RHISwapChain> swapChain, glm::vec4 backgroundColor)
	{
		m_Direct3DDeviceIMContext->ClearRenderTargetView((ID3D11RenderTargetView*)swapChain->GetRHIRenderTargetView(), glm::value_ptr(backgroundColor));
	}


	//===================End RHI Methods ==================//
}