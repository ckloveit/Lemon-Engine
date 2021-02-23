#include "LemonPCH.h"
#include "D3D11Utils.h"
#include "D3D11DynamicRHI.h"

using namespace std;
namespace Lemon::D3D11
{

	//================TextureUtils=============================
	bool CreateTexture2D(D3D11DynamicRHI* D3D11RHI,
		const uint32_t width,
		const uint32_t height,
		const uint32_t channelCount,
		const uint32_t bitsPerChannel,
		const uint8_t numMips,
		const uint32_t arraySize,
		const DXGI_FORMAT format,
		const UINT bindFlags,
		std::vector<std::vector<std::byte>>& data,
		ID3D11Texture2D*& outTexture)
	{
		const bool bHasInitialData = data.size() == numMips;

		// Desc
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = static_cast<UINT>(width);
		textureDesc.Height = static_cast<UINT>(height);
		textureDesc.MipLevels = static_cast<UINT>(numMips);
		textureDesc.ArraySize = arraySize;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = bHasInitialData ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = bindFlags;
		textureDesc.MiscFlags = 0;
		textureDesc.CPUAccessFlags = 0;

		//Set initial data
		vector<D3D11_SUBRESOURCE_DATA> subresourceDatas;
		if (bHasInitialData)
		{
			for (uint8_t i = 0; i < numMips; i++)
			{
				D3D11_SUBRESOURCE_DATA& subresource_data = subresourceDatas.emplace_back(D3D11_SUBRESOURCE_DATA{});
				subresource_data.pSysMem = i < data.size() ? data[i].data() : nullptr;        // Data pointer
				subresource_data.SysMemPitch = (width >> i) * channelCount * (bitsPerChannel / 8); // Line width in bytes
				subresource_data.SysMemSlicePitch = 0;  // This is only used for 3D textures
			}
		}
		return DXGIErrorCheck(D3D11RHI->GetDevice()->CreateTexture2D(&textureDesc, subresourceDatas.data(), reinterpret_cast<ID3D11Texture2D * *>(&outTexture)));
	}

	bool CreateRenderTargetView2D(D3D11DynamicRHI* D3D11RHI,
		ID3D11Texture2D* texture,
		const uint32_t arraySize,
		const DXGI_FORMAT format,
		std::vector<ID3D11RenderTargetView*>& outRTVs)
	{
		outRTVs.resize(arraySize);

		// Describe
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = format;
		rtvDesc.ViewDimension = (arraySize == 1) ? D3D11_RTV_DIMENSION_TEXTURE2D : D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.MipSlice = 0;
		rtvDesc.Texture2DArray.ArraySize = 1;
		rtvDesc.Texture2DArray.FirstArraySlice = 0;

		// Create
		for (uint32_t i = 0; i < arraySize; i++)
		{
			rtvDesc.Texture2DArray.FirstArraySlice = i;
			const auto result = D3D11RHI->GetDevice()->CreateRenderTargetView(static_cast<ID3D11Resource*>(texture), &rtvDesc, reinterpret_cast<ID3D11RenderTargetView **>(&outRTVs[i]));
			if (FAILED(result))
			{
				LEMON_CORE_ERROR("Failed, {0}.", DXGIErrorToString(result));
				return false;
			}
		}
		return true;
	}

	bool CreateShaderResourceView2D(D3D11DynamicRHI* D3D11RHI,
		ID3D11Texture2D* texture,
		const uint32_t arraySize,
		const uint8_t numMips,
		const DXGI_FORMAT format,
		ID3D11ShaderResourceView*& outSRV)
	{
		// Describe
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = format;
		srvDesc.ViewDimension = (arraySize == 1) ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.MipLevels = numMips;
		srvDesc.Texture2DArray.ArraySize = arraySize;

		// Create
		auto result = D3D11RHI->GetDevice()->CreateShaderResourceView(static_cast<ID3D11Resource*>(texture),
			&srvDesc, reinterpret_cast<ID3D11ShaderResourceView**>(&outSRV));
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("Failed, {0}.", DXGIErrorToString(result));
			return false;
		}
		return true;
	}

	bool CreateDepthStencilView2D(D3D11DynamicRHI* D3D11RHI,
		ID3D11Texture2D* texture,
		const uint32_t arraySize,
		const uint8_t numMips,
		const DXGI_FORMAT format,
		ID3D11DepthStencilView*& outDSV)
	{
		// Describe
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = format;
		dsvDesc.ViewDimension = (arraySize == 1) ? D3D11_DSV_DIMENSION_TEXTURE2D : D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		dsvDesc.Texture2DArray.MipSlice = 0;
		dsvDesc.Texture2DArray.ArraySize = 1;
		dsvDesc.Texture2DArray.FirstArraySlice = 0;
		dsvDesc.Flags = 0;

		// Create
		// Create
		auto result = D3D11RHI->GetDevice()->CreateDepthStencilView(static_cast<ID3D11Resource*>(texture),
			&dsvDesc, reinterpret_cast<ID3D11DepthStencilView **>(&outDSV));
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("Failed, {0}.", DXGIErrorToString(result));
			return false;
		}
		return true;
	}



}