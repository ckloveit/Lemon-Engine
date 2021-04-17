#include "LemonPCH.h"
#include "ImageImporter.h"
#include "Utils/FileUtils.h"
#include "Log/Log.h"
#include <std_image/std_image.h>

namespace Lemon
{
	inline ERHIPixelFormat GetPixelFormat(uint32_t imageBytesPerChannel, int channelCount)
	{
		if (channelCount == 1)
		{
			if (imageBytesPerChannel == 1) return RHI_PF_R8_Unorm;
			if (imageBytesPerChannel == 2) return RHI_PF_R16_Float;
			if (imageBytesPerChannel == 4) return RHI_PF_R32_Float;
		}
		else if (channelCount == 2)
		{
			if (imageBytesPerChannel == 1) return RHI_PF_R8G8_Unorm;
			if (imageBytesPerChannel == 2) return RHI_PF_R16G16_Float;
			if (imageBytesPerChannel == 4) return RHI_PF_R32G32_Float;
		}
		else if (channelCount == 3)
		{
			if (imageBytesPerChannel == 1) return RHI_PF_R32G32B32_Float;
			if (imageBytesPerChannel == 2) return RHI_PF_R32G32B32_Float;
			if (imageBytesPerChannel == 4) return RHI_PF_R32G32B32_Float;
		}
		else if (channelCount == 4)
		{
			if (imageBytesPerChannel == 1) return RHI_PF_R8G8B8A8_Unorm;
			if (imageBytesPerChannel == 2) return RHI_PF_R16G16B16A16_Float;
			if (imageBytesPerChannel == 4) return RHI_PF_R32G32B32A32_Float;
		}
		return ERHIPixelFormat::RHI_PF_Undefined;
	}

	ImageImporter::ImageImporter(Engine* engine)
		:m_Engine(engine)
	{

	}

	ImageImporter::~ImageImporter()
	{

	}
	bool ImageImporter::LoadImage(const std::string& filePath, TextureInfoData& OutTextureRawData, bool bGenerateMipmaps /* = false */)
	{
		if (!FileUtils::PathExists(filePath))
		{
			LEMON_CORE_ERROR("Path \"{0}\" is invalid.", filePath.c_str());
			return false;
		}
		int numComponentsRequested = 4; // don't care; we support 3 (RGB) or 4 (RGBA)
		int imageWidth;
		int imageHeight;

		// Deduce image properties
		uint32_t imageBytesPerChannel = 1;
		int imageChannelCount = 0; // Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)

		void* imageData = nullptr;

		const char* filePathName = filePath.c_str();
		if (stbi_is_16_bit(filePathName))
		{
			imageData = stbi_load_16(filePathName, &imageWidth, &imageHeight, &imageChannelCount, numComponentsRequested);
			imageBytesPerChannel = 2;
		}
		else if (stbi_is_hdr(filePathName))
		{
			imageData = stbi_loadf(filePathName, &imageWidth, &imageHeight, &imageChannelCount, numComponentsRequested);
			imageBytesPerChannel = 4;
		}
		else
		{
			numComponentsRequested = 4;
			imageData = stbi_load(filePathName, &imageWidth, &imageHeight, &imageChannelCount, numComponentsRequested);
			imageBytesPerChannel = 1;
			imageChannelCount = numComponentsRequested;
			
		}
		if (imageData == nullptr)
		{
			LEMON_CORE_ERROR("this file format is not support...");
			return false;
		}
		if (numComponentsRequested == 4)
		{
			// which means exist alpha channel

		}
		// get current texture format
		const ERHIPixelFormat imageFormat = GetPixelFormat(imageBytesPerChannel, imageChannelCount);

		OutTextureRawData.ChannelCount = imageChannelCount;
		OutTextureRawData.PerChannelBits = imageBytesPerChannel;
		OutTextureRawData.Width = imageWidth;
		OutTextureRawData.Height = imageHeight;
		OutTextureRawData.Format = imageFormat;

		TextureMipData& mipData = OutTextureRawData.RawData.AddMip();
		if (!FillImageData(&mipData.TextureData, imageData, imageWidth, imageHeight, imageChannelCount, imageBytesPerChannel))
		{
			LEMON_CORE_ERROR("FillImageData Failed......");
			return false;
		}

		if (bGenerateMipmaps)
		{
			// not yet consider
		}

		// Free memory
		stbi_image_free(imageData);
		return true;
	}

	bool ImageImporter::FillImageData(std::vector<std::byte>* data, void* ImageData, uint32_t width, uint32_t height, uint32_t channels, uint32_t perChannelBytes) const
	{
		if (!data || width == 0 || height == 0 || channels == 0)
		{
			LEMON_CORE_ERROR("Invalid Image Data");
			return false;
		}
		// Compute the expected data size
		const size_t size_bytes = width * height * channels * perChannelBytes;
		if (size_bytes != data->size())
		{
			data->clear();
			data->reserve(size_bytes);
			data->resize(size_bytes);
		}
		memcpy(&(*data)[0], ImageData, size_bytes);
		return true;
	}

}