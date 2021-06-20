#include "LemonPCH.h"
#include "EnvironmentComponent.h"
#include "Resources/ResourceSystem.h"
#include "Resources/Importer/ImageImporter.h"
#include "World/Entity.h"
#include "World/World.h"

namespace Lemon
{
	void EnvironmentComponent::CreateFromFilePath(const std::vector<std::string>& filePaths)
	{
		if (filePaths.empty() || filePaths.size() != 6)
			return;

		ResourceSystem* resourceSystem = m_Entity.GetWorld()->GetEngine()->GetSystem<ResourceSystem>();
		if (!resourceSystem)
		{
			LEMON_CORE_ERROR("resourceSystem is not Exist");
			return;
		}
		std::vector<TextureInfoData> textureInfoDatas;
		for (int i = 0; i < filePaths.size(); i++)
		{
			textureInfoDatas.emplace_back(TextureInfoData());
			resourceSystem->GetImageImporter()->LoadImage(filePaths[i], textureInfoDatas[i]);
		}

		//
		RHITextureCubeCreateInfo createInfo;
		for (int i = 0; i < textureInfoDatas.size(); i++)
		{
			createInfo.TextureArrayDatas.emplace_back(textureInfoDatas[i].RawData);
		}
		int envNumMips = 1;
		m_EnvTextureRHI = RHICreateTextureCube(textureInfoDatas[0].Width, textureInfoDatas[0].Height, textureInfoDatas[0].Format,
			envNumMips, RHI_TexCreate_ShaderResource, createInfo);
	}

	void EnvironmentComponent::InitEnvDiffuseIrradianceTexture()
	{
		if (!m_EnvTextureRHI)
		{
			return;
		}

		if (!m_EnvDiffuseIrradianceTexture)
		{
			uint32_t sizeX = m_EnvTextureRHI->GetSizeX();
			uint32_t sizeY = m_EnvTextureRHI->GetSizeY();
			ERHIPixelFormat pixelFormat = RHI_PF_R16G16B16A16_Float;// m_EnvironmentTextureRHI->GetPixelFormat();
			
			RHITextureCubeCreateInfo createInfo;
			m_EnvDiffuseIrradianceTexture = RHICreateTextureCube(sizeX, sizeY, pixelFormat, 1, 
				RHI_TexCreate_RenderTargetable | RHI_TexCreate_ShaderResource,
				createInfo);
		}

	}


	void EnvironmentComponent::InitEnvSpecularPrefilterTexture()
	{
		if (!m_EnvTextureRHI)
		{
			return;
		}

		if (!m_EnvSpecularPrefilterTexture)
		{
			uint32_t sizeX = m_EnvTextureRHI->GetSizeX() / 2;
			uint32_t sizeY = m_EnvTextureRHI->GetSizeY() / 2;
			ERHIPixelFormat pixelFormat = RHI_PF_R16G16B16A16_Float;// m_EnvironmentTextureRHI->GetPixelFormat();

			RHITextureCubeCreateInfo createInfo;
			m_EnvSpecularPrefilterTexture = RHICreateTextureCube(sizeX, sizeY, pixelFormat, 5,
				RHI_TexCreate_RenderTargetable | RHI_TexCreate_ShaderResource,
				createInfo);
		}
	}


	void EnvironmentComponent::InitEnvSpecularIntegrateBRDFTexture()
	{
		if (!m_EnvTextureRHI)
		{
			return;
		}
		if (!m_EnvSpecularIntegrateBRDFTexture)
		{
			uint32_t sizeX = m_EnvTextureRHI->GetSizeX();
			uint32_t sizeY = m_EnvTextureRHI->GetSizeY();
			ERHIPixelFormat pixelFormat = RHI_PF_R16G16B16A16_Float;// m_EnvironmentTextureRHI->GetPixelFormat();

			RHIResourceCreateInfo createInfo;
			m_EnvSpecularIntegrateBRDFTexture = RHICreateTexture2D(sizeX, sizeY, pixelFormat, 1,
				RHI_TexCreate_RenderTargetable | RHI_TexCreate_ShaderResource,
				createInfo);
		}
	}
}