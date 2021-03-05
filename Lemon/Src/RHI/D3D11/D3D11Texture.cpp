#include "LemonPCH.h"
#include "RenderCore/RenderUtils.h"
#include "D3D11Resources.h"
#include "D3D11DynamicRHI.h"

namespace Lemon
{
	bool IsShaderResource(uint32_t createFlags) { return createFlags & RHI_TexCreate_ShaderResource; }
	bool IsRenderTarget(uint32_t createFlags) { return createFlags & RHI_TexCreate_RenderTargetable; }
	bool IsDepthStencil(uint32_t createFlags) { return createFlags & RHI_TexCreate_DepthStencilTargetable; }


	// Create D3D11 Texture2D
	Ref<RHITexture2D> D3D11DynamicRHI::RHICreateTexture2D(uint32_t sizeX, uint32_t sizeY,
		ERHIPixelFormat format, uint32_t numMips, 
		uint32_t createFlags, RHIResourceCreateInfo& createInfo)
	{
		UINT bindFlags = D3D11::GetD3D11TextureResourceBindFlags(createFlags);

		DXGI_FORMAT D3Dformat = D3D11::GetD3D11TextureFormat(format);
		DXGI_FORMAT D3DformatDSV = D3D11::GetD3D11TextureFormatDSV(format);
		DXGI_FORMAT D3DformatSRV = D3D11::GetD3D11TextureFormatSRV(format);

		int channelCount = GPixelFormats[format].NumComponents;
		int perChannelBits = GPixelFormats[format].NumBytes / channelCount;

		bool bTexResult = true;
		bool bSRVResult = true;
		bool bRTVResult = true;
		bool bDSVResult = true;
		ID3D11Texture2D* texture = nullptr;
		bTexResult = D3D11::CreateTexture2D(this, sizeX, sizeY, channelCount, perChannelBits, numMips, 1, D3Dformat, bindFlags, createInfo.RawTextureDatas, texture);

		ID3D11ShaderResourceView* textureSRV = nullptr;
		if (IsShaderResource(createFlags))
		{
			bSRVResult = D3D11::CreateShaderResourceView2D(this, texture, 1, numMips, D3DformatSRV, textureSRV);
		}
		std::vector<ID3D11RenderTargetView*> textureRTVs;
		if (IsRenderTarget(createFlags))
		{
			bRTVResult = D3D11::CreateRenderTargetView2D(this, texture, 1, D3Dformat, textureRTVs);
		}
		ID3D11DepthStencilView* textureDSV = nullptr;
		if (IsDepthStencil(createFlags))
		{
			bDSVResult = D3D11::CreateDepthStencilView2D(this, texture, 1, numMips, D3DformatDSV, textureDSV);
		}
		if (bTexResult && bSRVResult && bRTVResult && bDSVResult)
		{
			return CreateRef<D3D11Texture2D>(this, texture, textureSRV, textureRTVs, textureDSV, sizeX, sizeY, numMips, format);
		}
		return nullptr;
	}



}