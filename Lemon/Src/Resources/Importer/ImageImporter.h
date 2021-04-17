#pragma once
#include "Core/Core.h"
#include "RHI/RHI.h"
#include "Resources/ResourceSystem.h"

namespace Lemon
{
	class Engine;
	class LEMON_API ImageImporter
	{
	public:
		ImageImporter(Engine* engine);
		~ImageImporter();

		bool LoadImage(const std::string& filePath, TextureInfoData& OutTextureRawData, bool bGenerateMipmaps = false);
	private:
		bool FillImageData(std::vector<std::byte>* data, void* ImageData, uint32_t width, uint32_t height, uint32_t channels, uint32_t perChannelBytes) const;

	private:
		Engine* m_Engine = nullptr;
	};
}