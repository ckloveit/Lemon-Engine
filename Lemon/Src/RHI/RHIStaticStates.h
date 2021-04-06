#include "Core/Core.h"
#include "DynamicRHI.h"
#include "RHI.h"
#include "RHIResources.h"
#include "RHIDefinitions.h"
#include <glm/vec4.hpp>
#include "Log/Log.h"
#include <array>

namespace Lemon
{
    /**
    * A static RHI sampler state resource.
    * TStaticSamplerStateRHI<...>::GetStaticState() will return a FSamplerStateRHIRef to a sampler state with the desired settings.
    * Should only be used from the rendering thread.
    */
    template<ESamplerFilter Filter=SF_Point,
        ESamplerAddressMode AddressU=AM_Clamp,
        ESamplerAddressMode AddressV=AM_Clamp,
        ESamplerAddressMode AddressW=AM_Clamp, 
        int32_t MipBias = 0,
        // Note: setting to a different value than GSystemSettings.MaxAnisotropy is only supported in D3D11 (is that still true?)
        // A value of 0 will use GSystemSettings.MaxAnisotropy
        int32_t MaxAnisotropy = 1,
        float BorderColor[4] = (0, 0, 0 ,0),
        /** Only supported in D3D11 */
        ESamplerCompareFunction SamplerComparisonFunction=SCF_Never>
    class TStaticSamplerState
    {
    public:
        static Ref<RHISamplerState> CreateRHI()
        {
            const glm::vec4 borderColor = glm::vec4(BorderColor[0],BorderColor[1],BorderColor[2],BorderColor[3] );
            const SamplerStateInitializer initializer( Filter, AddressU, AddressV, AddressW, MipBias, MaxAnisotropy, 0, FLT_MAX, borderColor, SamplerComparisonFunction );
            return RHICreateSamplerState(initializer);
        }
    };
    
    /**
    * A static RHI rasterizer state resource.
    * TStaticRasterizerStateRHI<...>::GetStaticState() will return a FRasterizerStateRHIRef to a rasterizer state with the desired
    * settings.
    * Should only be used from the rendering thread.
    */
    template<ERasterizerFillMode FillMode= RFM_Solid,ERasterizerCullMode CullMode=RCM_None,bool bEnableLineAA=false,bool bEnableMSAA=false>
    class TStaticRasterizerState
    {
    public:
        static Ref<RHIRasterizerState> CreateRHI()
        {
            const RasterizerStateInitializer initializer = { FillMode, CullMode };
            return RHICreateRasterizerState(initializer);
        }
    };

    /**
    * A static RHI stencil state resource.
    * TStaticStencilStateRHI<...>::GetStaticState() will return a FDepthStencilStateRHIRef to a stencil state with the desired
    * settings.
    * Should only be used from the rendering thread.
    */
    template<
        bool bEnableDepthWrite = true,
        ECompareFunction DepthTest = CF_LessEqual,
        bool bEnableFrontFaceStencil = false,
        ECompareFunction FrontFaceStencilTest = CF_Always,
        EStencilOp FrontFaceStencilFailStencilOp = SO_Keep,
        EStencilOp FrontFaceDepthFailStencilOp = SO_Keep,
        EStencilOp FrontFacePassStencilOp = SO_Keep,
        bool bEnableBackFaceStencil = false,
        ECompareFunction BackFaceStencilTest = CF_Always,
        EStencilOp BackFaceStencilFailStencilOp = SO_Keep,
        EStencilOp BackFaceDepthFailStencilOp = SO_Keep,
        EStencilOp BackFacePassStencilOp = SO_Keep,
        uint8_t StencilReadMask = 0xFF,
        uint8_t StencilWriteMask = 0xFF
        >
    class TStaticDepthStencilState
    {
    public:
        static Ref<RHIDepthStencilState> CreateRHI()
        {
            DepthStencilStateInitializer initializer(
                bEnableDepthWrite,
                DepthTest,
                bEnableFrontFaceStencil,
                FrontFaceStencilTest,
                FrontFaceStencilFailStencilOp,
                FrontFaceDepthFailStencilOp,
                FrontFacePassStencilOp,
                bEnableBackFaceStencil,
                BackFaceStencilTest,
                BackFaceStencilFailStencilOp,
                BackFaceDepthFailStencilOp,
                BackFacePassStencilOp,
                StencilReadMask,
                StencilWriteMask);

            return RHICreateDepthStencilState(initializer);
        }
    };

    /**
	 * A static RHI blend state resource.
	 * TStaticBlendStateRHI<...>::GetStaticState() will return a FBlendStateRHIRef to a blend state with the desired settings.
	 * Should only be used from the rendering thread.
	 * 
	 * Alpha blending happens on GPU's as:
	 * FinalColor.rgb = SourceColor * ColorSrcBlend (ColorBlendOp) DestColor * ColorDestBlend;
	 * if (BlendState->bSeparateAlphaBlendEnable)
	 *		FinalColor.a = SourceAlpha * AlphaSrcBlend (AlphaBlendOp) DestAlpha * AlphaDestBlend;
	 * else
	 *		Alpha blended the same way as rgb
	 * 
	 * Where source is the color coming from the pixel shader, and target is the color in the render target.
	 *
	 * So for example, TStaticBlendState<BO_Add,BF_SourceAlpha,BF_InverseSourceAlpha,BO_Add,BF_Zero,BF_One> produces:
	 * FinalColor.rgb = SourceColor * SourceAlpha + DestColor * (1 - SourceAlpha);
	 * FinalColor.a = SourceAlpha * 0 + DestAlpha * 1;
	 */
	template<
		EColorWriteMask RT0ColorWriteMask = CW_RGBA,
		EBlendOperation RT0ColorBlendOp = BO_Add,
		EBlendFactor    RT0ColorSrcBlend = BF_One,
		EBlendFactor    RT0ColorDestBlend = BF_Zero,
		EBlendOperation RT0AlphaBlendOp = BO_Add,
		EBlendFactor    RT0AlphaSrcBlend = BF_One,
		EBlendFactor    RT0AlphaDestBlend = BF_Zero,
		EColorWriteMask RT1ColorWriteMask = CW_RGBA,
		EBlendOperation RT1ColorBlendOp = BO_Add,
		EBlendFactor    RT1ColorSrcBlend = BF_One,
		EBlendFactor    RT1ColorDestBlend = BF_Zero,
		EBlendOperation RT1AlphaBlendOp = BO_Add,
		EBlendFactor    RT1AlphaSrcBlend = BF_One,
		EBlendFactor    RT1AlphaDestBlend = BF_Zero,
		EColorWriteMask RT2ColorWriteMask = CW_RGBA,
		EBlendOperation RT2ColorBlendOp = BO_Add,
		EBlendFactor    RT2ColorSrcBlend = BF_One,
		EBlendFactor    RT2ColorDestBlend = BF_Zero,
		EBlendOperation RT2AlphaBlendOp = BO_Add,
		EBlendFactor    RT2AlphaSrcBlend = BF_One,
		EBlendFactor    RT2AlphaDestBlend = BF_Zero,
		EColorWriteMask RT3ColorWriteMask = CW_RGBA,
		EBlendOperation RT3ColorBlendOp = BO_Add,
		EBlendFactor    RT3ColorSrcBlend = BF_One,
		EBlendFactor    RT3ColorDestBlend = BF_Zero,
		EBlendOperation RT3AlphaBlendOp = BO_Add,
		EBlendFactor    RT3AlphaSrcBlend = BF_One,
		EBlendFactor    RT3AlphaDestBlend = BF_Zero,
		EColorWriteMask RT4ColorWriteMask = CW_RGBA,
		EBlendOperation RT4ColorBlendOp = BO_Add,
		EBlendFactor    RT4ColorSrcBlend = BF_One,
		EBlendFactor    RT4ColorDestBlend = BF_Zero,
		EBlendOperation RT4AlphaBlendOp = BO_Add,
		EBlendFactor    RT4AlphaSrcBlend = BF_One,
		EBlendFactor    RT4AlphaDestBlend = BF_Zero,
		EColorWriteMask RT5ColorWriteMask = CW_RGBA,
		EBlendOperation RT5ColorBlendOp = BO_Add,
		EBlendFactor    RT5ColorSrcBlend = BF_One,
		EBlendFactor    RT5ColorDestBlend = BF_Zero,
		EBlendOperation RT5AlphaBlendOp = BO_Add,
		EBlendFactor    RT5AlphaSrcBlend = BF_One,
		EBlendFactor    RT5AlphaDestBlend = BF_Zero,
		EColorWriteMask RT6ColorWriteMask = CW_RGBA,
		EBlendOperation RT6ColorBlendOp = BO_Add,
		EBlendFactor    RT6ColorSrcBlend = BF_One,
		EBlendFactor    RT6ColorDestBlend = BF_Zero,
		EBlendOperation RT6AlphaBlendOp = BO_Add,
		EBlendFactor    RT6AlphaSrcBlend = BF_One,
		EBlendFactor    RT6AlphaDestBlend = BF_Zero,
		EColorWriteMask RT7ColorWriteMask = CW_RGBA,
		EBlendOperation RT7ColorBlendOp = BO_Add,
		EBlendFactor    RT7ColorSrcBlend = BF_One,
		EBlendFactor    RT7ColorDestBlend = BF_Zero,
		EBlendOperation RT7AlphaBlendOp = BO_Add,
		EBlendFactor    RT7AlphaSrcBlend = BF_One,
		EBlendFactor    RT7AlphaDestBlend = BF_Zero
		>
	class TStaticBlendState
	{
	public:
		static Ref<RHIBlendState> CreateRHI()
		{
			BlendStateInitializer initializer;
			initializer.RenderTargets[0] = BlendStateInitializer::RenderTargetBlendState(RT0ColorBlendOp,RT0ColorSrcBlend,RT0ColorDestBlend,RT0AlphaBlendOp,RT0AlphaSrcBlend,RT0AlphaDestBlend,RT0ColorWriteMask);
			initializer.RenderTargets[1] = BlendStateInitializer::RenderTargetBlendState(RT1ColorBlendOp,RT1ColorSrcBlend,RT1ColorDestBlend,RT1AlphaBlendOp,RT1AlphaSrcBlend,RT1AlphaDestBlend,RT1ColorWriteMask);
			initializer.RenderTargets[2] = BlendStateInitializer::RenderTargetBlendState(RT2ColorBlendOp,RT2ColorSrcBlend,RT2ColorDestBlend,RT2AlphaBlendOp,RT2AlphaSrcBlend,RT2AlphaDestBlend,RT2ColorWriteMask);
			initializer.RenderTargets[3] = BlendStateInitializer::RenderTargetBlendState(RT3ColorBlendOp,RT3ColorSrcBlend,RT3ColorDestBlend,RT3AlphaBlendOp,RT3AlphaSrcBlend,RT3AlphaDestBlend,RT3ColorWriteMask);
			initializer.RenderTargets[4] = BlendStateInitializer::RenderTargetBlendState(RT4ColorBlendOp,RT4ColorSrcBlend,RT4ColorDestBlend,RT4AlphaBlendOp,RT4AlphaSrcBlend,RT4AlphaDestBlend,RT4ColorWriteMask);
			initializer.RenderTargets[5] = BlendStateInitializer::RenderTargetBlendState(RT5ColorBlendOp,RT5ColorSrcBlend,RT5ColorDestBlend,RT5AlphaBlendOp,RT5AlphaSrcBlend,RT5AlphaDestBlend,RT5ColorWriteMask);
			initializer.RenderTargets[6] = BlendStateInitializer::RenderTargetBlendState(RT6ColorBlendOp,RT6ColorSrcBlend,RT6ColorDestBlend,RT6AlphaBlendOp,RT6AlphaSrcBlend,RT6AlphaDestBlend,RT6ColorWriteMask);
			initializer.RenderTargets[7] = BlendStateInitializer::RenderTargetBlendState(RT7ColorBlendOp,RT7ColorSrcBlend,RT7ColorDestBlend,RT7AlphaBlendOp,RT7AlphaSrcBlend,RT7AlphaDestBlend,RT7ColorWriteMask);

			return RHICreateBlendState(initializer);
		}
	};

	/**
	* A static RHI blend state resource which only allows controlling MRT write masks, for use when only opaque blending is needed.
	*/
	template<
        EColorWriteMask RT0ColorWriteMask = CW_RGBA,
        EColorWriteMask RT1ColorWriteMask = CW_RGBA,
        EColorWriteMask RT2ColorWriteMask = CW_RGBA,
        EColorWriteMask RT3ColorWriteMask = CW_RGBA,
        EColorWriteMask RT4ColorWriteMask = CW_RGBA,
        EColorWriteMask RT5ColorWriteMask = CW_RGBA,
        EColorWriteMask RT6ColorWriteMask = CW_RGBA,
        EColorWriteMask RT7ColorWriteMask = CW_RGBA
        >
    class TStaticBlendStateWriteMask : public TStaticBlendState<
        RT0ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
        RT1ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
        RT2ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
        RT3ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
        RT4ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
        RT5ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
        RT6ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
        RT7ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero
        >
	{
	public:
		static Ref<RHIBlendState> CreateRHI()
		{
			return TStaticBlendState<
                RT0ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
                RT1ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
                RT2ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
                RT3ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
                RT4ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
                RT5ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
                RT6ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero,
                RT7ColorWriteMask,BO_Add,BF_One,BF_Zero,BO_Add,BF_One,BF_Zero
                >::CreateRHI();
		}
	};

}

