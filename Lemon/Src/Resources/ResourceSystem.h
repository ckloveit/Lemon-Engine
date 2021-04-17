#pragma once
#include <array>
#include "Core/ISystem.h"
#include "Core/Engine.h"
#include <std_image.h>
#include "RHI/RHI.h"

namespace Lemon
{
	class ImageImporter;

	struct TextureInfoData
	{
		TextureRawData RawData;
		uint32_t Width;
		uint32_t Height;
		uint32_t ChannelCount;
		uint32_t PerChannelBits;
		ERHIPixelFormat Format;
	};

	enum AssetType
	{
		Asset_Cubemaps,
		Asset_Fonts,
		Asset_Icons,
		Asset_Scripts,
		Asset_ShaderCompiler,
		Asset_Shaders,
		Asset_Textures,
		Asset_Model
	};

	class LEMON_API ResourceSystem : public ISystem
	{
	public:
		ResourceSystem(Engine* engine);
		~ResourceSystem() = default;

		virtual bool Initialize() override;

		void AddDataDirectory(AssetType type, const std::string& directory);
		std::string GetAssetDataDirectory(AssetType type);

		// Importers
		auto GetImageImporter() const { return m_ImageImporter.get(); }
	private:
		std::map<AssetType, std::string> m_ResourceDirectoriesMap;

		//Importer 
		std::shared_ptr<ImageImporter> m_ImageImporter;
	};
}