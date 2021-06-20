#include "LemonPCH.h"
#include "D3D11Utils.h"
#include "D3D11DynamicRHI.h"

using namespace std;
namespace Lemon::D3D11
{

	//================TextureUtils=============================

	//========Texture2D=================
	bool CreateTexture2D(D3D11DynamicRHI* D3D11RHI,
		const uint32_t width,
		const uint32_t height,
		const uint32_t channelCount,
		const uint32_t bitsPerChannel,
		const uint8_t numMips,
		const DXGI_FORMAT format,
		const UINT bindFlags,
		TextureRawData& data,
		ID3D11Texture2D*& outTexture)
	{
		const bool bHasInitialData = data.MipDatas.size() == numMips;

		// DESC
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = static_cast<UINT>(width);
		textureDesc.Height = static_cast<UINT>(height);
		textureDesc.MipLevels = static_cast<UINT>(numMips);
		textureDesc.ArraySize = 1;
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
				D3D11_SUBRESOURCE_DATA& subresourceData = subresourceDatas.emplace_back(D3D11_SUBRESOURCE_DATA{});
				subresourceData.pSysMem = i < data.MipDatas.size() ? data.MipDatas[i].TextureData.data() : nullptr;        // Data pointer
				subresourceData.SysMemPitch = (width >> i) * channelCount * bitsPerChannel;// (bitsPerChannel / 8); // Line width in bytes
				subresourceData.SysMemSlicePitch = 0;  // This is only used for 3D textures
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


	//========Texture2DArray=================
	bool CreateTexture2DArray(D3D11DynamicRHI* D3D11RHI,
		const uint32_t width,
		const uint32_t height,
		const uint32_t arraySize,
		const uint32_t channelCount,
		const uint32_t bitsPerChannel,
		const uint8_t numMips,
		const DXGI_FORMAT format,
		const UINT bindFlags,
		std::vector<TextureRawData>& arrayDatas,
		ID3D11Texture2D*& outTexture)
	{
		const bool bHasInitialData = arrayDatas.size() == arraySize;

		// DESC
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
			for (int arrayIndex = 0; arrayIndex < arraySize; arrayIndex++)
			{
				for (uint8_t i = 0; i < numMips; i++)
				{
					D3D11_SUBRESOURCE_DATA& subresourceData = subresourceDatas.emplace_back(D3D11_SUBRESOURCE_DATA{});
					subresourceData.pSysMem = i < arrayDatas[arrayIndex].MipDatas.size() ? arrayDatas[arrayIndex].MipDatas[i].TextureData.data() : nullptr;        // Data pointer
					subresourceData.SysMemPitch = (width >> i) * channelCount * (bitsPerChannel / 8); // Line width in bytes
					subresourceData.SysMemSlicePitch = subresourceData.SysMemPitch * (height >> i);  // This is only used for 3D textures or texture2DArray
				}
			}
		}
		return DXGIErrorCheck(D3D11RHI->GetDevice()->CreateTexture2D(&textureDesc, subresourceDatas.data(), reinterpret_cast<ID3D11Texture2D * *>(&outTexture)));
	}


	//===========TextureCube========================//
	bool CreateTextureCube(D3D11DynamicRHI* D3D11RHI,
		const uint32_t width,
		const uint32_t height,
		const uint32_t channelCount,
		const uint32_t bitsPerChannel,
		const uint8_t numMips,
		const DXGI_FORMAT format,
		const UINT bindFlags,
		std::vector<TextureRawData>& arrayDatas,
		ID3D11Texture2D*& outTexture)
	{
		// DESC
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = static_cast<UINT>(width);
		textureDesc.Height = static_cast<UINT>(height);
		textureDesc.MipLevels = static_cast<UINT>(numMips);
		textureDesc.ArraySize = 6;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = (bindFlags & D3D11_BIND_RENDER_TARGET) || (bindFlags & D3D11_BIND_DEPTH_STENCIL) ? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE;
		textureDesc.BindFlags = bindFlags;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		textureDesc.CPUAccessFlags = 0;

		vector<D3D11_SUBRESOURCE_DATA> subresourceDatas;
		vector<D3D11_TEXTURE2D_DESC> textureCubeDesc;

		if (arrayDatas.size() == 6)
		{
			for (uint32_t faceIndex = 0; faceIndex < 6; faceIndex++)
			{
				const auto& facedata = arrayDatas[faceIndex];

				if (facedata.MipDatas.size() == 0)
				{
					LEMON_CORE_ERROR("A side contains no data");
					continue;
				}

				for (uint32_t mipLevel = 0; mipLevel < static_cast<uint32_t>(facedata.MipDatas.size()); mipLevel++)
				{
					const auto& mipData = facedata.MipDatas[mipLevel];

					if (mipData.TextureData.size() == 0)
					{
						LEMON_CORE_ERROR("A mip-map contains invalid data.");
						continue;
					}

					// D3D11_SUBRESOURCE_DATA
					auto& subresourceData = subresourceDatas.emplace_back(D3D11_SUBRESOURCE_DATA{});
					subresourceData.pSysMem = mipData.TextureData.data();                                          // Data pointer
					subresourceData.SysMemPitch = (width >> mipLevel) * channelCount * (bitsPerChannel); // Line width in bytes
					subresourceData.SysMemSlicePitch = 0;                                                        // This is only used for 3D textures
				}
				textureCubeDesc.emplace_back(textureDesc);
			}
		}
		else
		{
			textureCubeDesc.emplace_back(textureDesc);
		}
		return DXGIErrorCheck(D3D11RHI->GetDevice()->CreateTexture2D(textureCubeDesc.data(), subresourceDatas.data(), reinterpret_cast<ID3D11Texture2D **>(&outTexture)));
	}

	bool CreateShaderResourceViewCube(D3D11DynamicRHI* D3D11RHI,
		ID3D11Texture2D* texture,
		const uint8_t numMips,
		const DXGI_FORMAT format,
		ID3D11ShaderResourceView*& outSRV)
	{
		// Describe
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = numMips;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.MipLevels = numMips;// 1;
		srvDesc.Texture2DArray.ArraySize = 6;

		// Create
		auto result = D3D11RHI->GetDevice()->CreateShaderResourceView(static_cast<ID3D11Resource*>(texture),
			&srvDesc, reinterpret_cast<ID3D11ShaderResourceView **>(&outSRV));
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("Failed, {0}.", DXGIErrorToString(result));
			return false;
		}
		return true;
	}

	bool CreateRenderTargetViewCube(D3D11DynamicRHI* D3D11RHI,
		ID3D11Texture2D* texture,
		const uint8_t numMips,
		const DXGI_FORMAT format,
		std::vector<ID3D11RenderTargetView*>& outSRVs)
	{
		if (outSRVs.size() != 6)
		{
			outSRVs.resize(6);
		}
		// Describe
		D3D11_RENDER_TARGET_VIEW_DESC viewDesc = {};
		viewDesc.Format = format;
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		viewDesc.Texture2DArray.MipSlice = 0;
		viewDesc.Texture2DArray.ArraySize = 1;
		viewDesc.Texture2DArray.FirstArraySlice = 0;

		// Create
		for (uint32_t i = 0; i < 6; i++)
		{
			viewDesc.Texture2DArray.FirstArraySlice = i;
			const auto result = D3D11RHI->GetDevice()->CreateRenderTargetView(static_cast<ID3D11Resource*>(texture), &viewDesc, reinterpret_cast<ID3D11RenderTargetView **>(&outSRVs[i]));
			if (FAILED(result))
			{
				LEMON_CORE_ERROR("Failed, {0}.", DXGIErrorToString(result));
				return false;
			}
		}
		return true;


	}

}