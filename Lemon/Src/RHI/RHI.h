#pragma once
#include "Core/Core.h"
#include "Containers/ResourceArray.h"
#include "RHIDefinitions.h"
#include <map>
#include <array>

namespace Lemon
{
	extern LEMON_API void RHIInit();
	extern LEMON_API void RHIExit();

	class RHISwapChain;
	struct RHIResourceCreateInfo
	{
		// for CreateTexture uses
		std::vector<std::vector<std::byte>> RawTextureDatas;
		// for CreateVertexBuffer & CreateStructBuffer uses
		ResourceArrayInterface* ResourceArray;
		// for Debug use
		std::string DebugName;
	};

	struct RHIShaderCreateInfo
	{
	public:
		FORCE_INLINE void AddDefine(const std::string& define, const std::string& value = "1") { ShaderDefines[define] = value; }
	public:
		std::map<std::string, std::string> ShaderDefines;
	};

	struct RHIVertexElement
	{
		uint8_t StreamIndex;
		uint8_t Offset;
		ERHIVertexElementType Type;
		uint8_t AttributeIndex;
		uint16_t Stride;
		/**
		 * Whether to use instance index or vertex index to consume the element.
		 * eg if bUseInstanceIndex is 0, the element will be repeated for every instance.
		 */
		uint16_t bUseInstanceIndex;

		RHIVertexElement() {}
		RHIVertexElement(uint8_t InStreamIndex, uint8_t InOffset, ERHIVertexElementType InType, uint8_t InAttributeIndex, uint16_t InStride, bool bInUseInstanceIndex = false) :
			StreamIndex(InStreamIndex),
			Offset(InOffset),
			Type(InType),
			AttributeIndex(InAttributeIndex),
			Stride(InStride),
			bUseInstanceIndex(bInUseInstanceIndex)
		{}

	};
	typedef std::vector<RHIVertexElement> VertexDeclarationElementList;


	inline uint32_t GetIndexCountForPrimitiveCount(uint32_t NumPrimitives, EPrimitiveType PrimitiveType)
	{
		//static_assert(PT_Num == 38, "This function needs to be updated");
		uint32_t Factor = (PrimitiveType == PT_TriangleList) ? 3 : 1;
		uint32_t Offset = 0;// (PrimitiveType == PT_TriangleStrip) ? 2 : 0;
		return NumPrimitives * Factor + Offset;
	}

}