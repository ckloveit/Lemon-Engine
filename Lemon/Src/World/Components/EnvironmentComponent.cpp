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
		
		m_EnvironmentTextureRHI = RHICreateTextureCube(textureInfoDatas[0].Width, textureInfoDatas[0].Height, textureInfoDatas[0].Format,
			1, RHI_TexCreate_ShaderResource, createInfo);
	}

	void EnvironmentComponent::InitEnvDiffuseIrradianceTexture()
	{
		if (!m_EnvironmentTextureRHI)
		{
			return;
		}

		if (!m_EnvironmentDiffuseIrradianceTexture)
		{
			uint32_t sizeX = m_EnvironmentTextureRHI->GetSizeX();
			uint32_t sizeY = m_EnvironmentTextureRHI->GetSizeY();
			ERHIPixelFormat pixelFormat = m_EnvironmentTextureRHI->GetPixelFormat();
			
			RHITextureCubeCreateInfo createInfo;
			m_EnvironmentDiffuseIrradianceTexture = RHICreateTextureCube(sizeX, sizeY, pixelFormat, 1, 
				RHI_TexCreate_RenderTargetable | RHI_TexCreate_ShaderResource,
				createInfo);
		}

	}
}