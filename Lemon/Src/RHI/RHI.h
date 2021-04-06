#pragma once
#include "Core/Core.h"
#include "Containers/ResourceArray.h"
#include "RHIDefinitions.h"
#include <map>
#include <array>

#include <glm/vec4.hpp>

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
	//Uniform Buffer will has valid size
	inline uint32_t GetValidateUniformBufferSize(uint32_t num) { return num + (16 - num % 16); }

	struct DepthStencilStateInitializer
	{
		bool bEnableDepthWrite;
		ECompareFunction DepthTest;

		bool bEnableFrontFaceStencil;
		ECompareFunction FrontFaceStencilTest;
		EStencilOp	FrontFaceStencilFailStencilOp;
		EStencilOp	FrontFaceDepthFailStencilOp;
		EStencilOp	FrontFacePassStencilOp;
		bool bEnableBackFaceStencil;
		ECompareFunction BackFaceStencilTest;
		EStencilOp	BackFaceStencilFailStencilOp;
		EStencilOp	BackFaceDepthFailStencilOp;
		EStencilOp	BackFacePassStencilOp;
		uint8_t StencilReadMask;
		uint8_t StencilWriteMask;

		DepthStencilStateInitializer(
			bool bInEnableDepthWrite = true,
    		ECompareFunction InDepthTest = CF_LessEqual,
    		bool bInEnableFrontFaceStencil = false,
    		ECompareFunction InFrontFaceStencilTest = CF_Always,
    		EStencilOp InFrontFaceStencilFailStencilOp = SO_Keep,
    		EStencilOp InFrontFaceDepthFailStencilOp = SO_Keep,
    		EStencilOp InFrontFacePassStencilOp = SO_Keep,
    		bool bInEnableBackFaceStencil = false,
    		ECompareFunction InBackFaceStencilTest = CF_Always,
    		EStencilOp InBackFaceStencilFailStencilOp = SO_Keep,
    		EStencilOp InBackFaceDepthFailStencilOp = SO_Keep,
    		EStencilOp InBackFacePassStencilOp = SO_Keep,
    		uint8_t InStencilReadMask = 0xFF,
    		uint8_t InStencilWriteMask = 0xFF)
		: bEnableDepthWrite(bInEnableDepthWrite)
	    , DepthTest(InDepthTest)
	    , bEnableFrontFaceStencil(bInEnableFrontFaceStencil)
	    , FrontFaceStencilTest(InFrontFaceStencilTest)
	    , FrontFaceStencilFailStencilOp(InFrontFaceStencilFailStencilOp)
	    , FrontFaceDepthFailStencilOp(InFrontFaceDepthFailStencilOp)
	    , FrontFacePassStencilOp(InFrontFacePassStencilOp)
	    , bEnableBackFaceStencil(bInEnableBackFaceStencil)
	    , BackFaceStencilTest(InBackFaceStencilTest)
	    , BackFaceStencilFailStencilOp(InBackFaceStencilFailStencilOp)
	    , BackFaceDepthFailStencilOp(InBackFaceDepthFailStencilOp)
	    , BackFacePassStencilOp(InBackFacePassStencilOp)
	    , StencilReadMask(InStencilReadMask)
	    , StencilWriteMask(InStencilWriteMask)
		{}

		std::string ToString() const;
	};

	struct RasterizerStateInitializer
	{
		ERasterizerFillMode FillMode;
		ERasterizerCullMode CullMode;
	};

	struct BlendStateInitializer
	{
		struct RenderTargetBlendState
		{
			enum
			{
				NUM_STRING_FIELDS = 7
            };
			EBlendOperation ColorBlendOp;
			EBlendFactor ColorSrcBlend;
			EBlendFactor ColorDestBlend;
			EBlendOperation AlphaBlendOp;
			EBlendFactor AlphaSrcBlend;
			EBlendFactor AlphaDestBlend;
			EColorWriteMask ColorWriteMask;
		
			RenderTargetBlendState(
                EBlendOperation InColorBlendOp = BO_Add,
                EBlendFactor InColorSrcBlend = BF_One,
                EBlendFactor InColorDestBlend = BF_Zero,
                EBlendOperation InAlphaBlendOp = BO_Add,
                EBlendFactor InAlphaSrcBlend = BF_One,
                EBlendFactor InAlphaDestBlend = BF_Zero,
                EColorWriteMask InColorWriteMask = CW_RGBA
                )
            : ColorBlendOp(InColorBlendOp)
            , ColorSrcBlend(InColorSrcBlend)
            , ColorDestBlend(InColorDestBlend)
            , AlphaBlendOp(InAlphaBlendOp)
            , AlphaSrcBlend(InAlphaSrcBlend)
            , AlphaDestBlend(InAlphaDestBlend)
            , ColorWriteMask(InColorWriteMask)
			{}
		};
		BlendStateInitializer()
			:bUseIndependentRenderTargetBlendStates(false){}
		BlendStateInitializer(const RenderTargetBlendState& InRenderTargetBlendState)
			:bUseIndependentRenderTargetBlendStates(false)
		{
			RenderTargets[0] = InRenderTargetBlendState;
		}
		template<uint32_t NumRenderTargets>
		BlendStateInitializer(std::array<RenderTargetBlendState, NumRenderTargets>& InRenderTargetBlendStates)
			:bUseIndependentRenderTargetBlendStates(NumRenderTargets > 1)
		{
			static_assert(NumRenderTargets <= MaxSimultaneousRenderTargets, "Too many render target blend states.");
			for(uint32_t RenderTargetIndex = 0;RenderTargetIndex < NumRenderTargets;++RenderTargetIndex)
			{
				RenderTargets[RenderTargetIndex] = InRenderTargetBlendStates[RenderTargetIndex];
			}
		}
		
		std::array<RenderTargetBlendState, MaxSimultaneousRenderTargets> RenderTargets;
		bool bUseIndependentRenderTargetBlendStates;
	};

	struct SamplerStateInitializer
	{
		SamplerStateInitializer() {}
		SamplerStateInitializer(
            ESamplerFilter InFilter,
            ESamplerAddressMode InAddressU = AM_Wrap,
            ESamplerAddressMode InAddressV = AM_Wrap,
            ESamplerAddressMode InAddressW = AM_Wrap,
            float InMipBias = 0,
            int32_t InMaxAnisotropy = 0,
            float InMinMipLevel = 0,
            float InMaxMipLevel = FLT_MAX,
            glm::vec4 InBorderColor = {0, 0, 0, 0 },
            /** Only supported in D3D11 */
            ESamplerCompareFunction InSamplerComparisonFunction = SCF_Never
            )
        :	Filter(InFilter)
        ,	AddressU(InAddressU)
        ,	AddressV(InAddressV)
        ,	AddressW(InAddressW)
        ,	MipBias(InMipBias)
        ,	MinMipLevel(InMinMipLevel)
        ,	MaxMipLevel(InMaxMipLevel)
        ,	MaxAnisotropy(InMaxAnisotropy)
        ,	BorderColor(InBorderColor)
        ,	SamplerComparisonFunction(InSamplerComparisonFunction)
		{
		}
		
		ESamplerFilter Filter;
        ESamplerAddressMode AddressU;
        ESamplerAddressMode AddressV;
        ESamplerAddressMode AddressW;
        float MipBias;
        /** Smallest mip map level that will be used, where 0 is the highest resolution mip level. */
        float MinMipLevel;
        /** Largest mip map level that will be used, where 0 is the highest resolution mip level. */
        float MaxMipLevel;
        int32_t MaxAnisotropy;
        glm::vec4 BorderColor;
        ESamplerCompareFunction SamplerComparisonFunction;
	};
}
