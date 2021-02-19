#pragma once

#include <d3d11_4.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <wrl/client.h>

namespace Lemon
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
}
namespace Lemon::D3D11
{
	static const DXGI_FORMAT D3D11Format[] =
	{
		// R
		DXGI_FORMAT_R8_UNORM,
		DXGI_FORMAT_R16_UINT,
		DXGI_FORMAT_R16_FLOAT,
		DXGI_FORMAT_R32_UINT,
		DXGI_FORMAT_R32_FLOAT,
		// RG
		DXGI_FORMAT_R8G8_UNORM,
		DXGI_FORMAT_R16G16_FLOAT,
		DXGI_FORMAT_R32G32_FLOAT,
		// RGB
		DXGI_FORMAT_R11G11B10_FLOAT,
		DXGI_FORMAT_R32G32B32_FLOAT,
		// RGBA
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R10G10B10A2_UNORM,
		DXGI_FORMAT_R16G16B16A16_SNORM,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		// Depth
		DXGI_FORMAT_D32_FLOAT,
		DXGI_FORMAT_D32_FLOAT_S8X24_UINT,

		DXGI_FORMAT_UNKNOWN
	};


}

#include "D3D11Helpers.h"

