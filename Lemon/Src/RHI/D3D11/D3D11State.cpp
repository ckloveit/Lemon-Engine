#include "LemonPCH.h"
#include "D3D11DynamicRHI.h"
#include "D3D11State.h"
namespace Lemon
{
	//SamplerState
	static std::map<ID3D11SamplerState*, Ref<RHISamplerState>> GSamplerStateCache;
	static std::map<ID3D11RasterizerState*, Ref<RHIRasterizerState>> GRasterizerStataCache;
	static std::map<ID3D11DepthStencilState*, Ref<RHIDepthStencilState>> GDepthStencilStateCache;
	static std::map<ID3D11BlendState*, Ref<RHIBlendState>> GBlendStateCache;
	
	void EmptyD3DSamplerStateCache()
	{
		for (auto Iter = GSamplerStateCache.begin(); Iter != GSamplerStateCache.end(); ++Iter )
		{
			auto& State = Iter->second;
			// Manually release
			delete State.get();
			State = nullptr;
		}
		GSamplerStateCache.clear();

		
	}
	
    static D3D11_COMPARISON_FUNC TranslateCompareFunction(ECompareFunction CompareFunction)
    {
        switch(CompareFunction)
        {
        case CF_Less: return D3D11_COMPARISON_LESS;
        case CF_LessEqual: return D3D11_COMPARISON_LESS_EQUAL;
        case CF_Greater: return D3D11_COMPARISON_GREATER;
        case CF_GreaterEqual: return D3D11_COMPARISON_GREATER_EQUAL;
        case CF_Equal: return D3D11_COMPARISON_EQUAL;
        case CF_NotEqual: return D3D11_COMPARISON_NOT_EQUAL;
        case CF_Never: return D3D11_COMPARISON_NEVER;
        default: return D3D11_COMPARISON_ALWAYS;
        };
    }
    static D3D11_STENCIL_OP TranslateStencilOp(EStencilOp StencilOp)
    {
        switch(StencilOp)
        {
        case SO_Zero: return D3D11_STENCIL_OP_ZERO;
        case SO_Replace: return D3D11_STENCIL_OP_REPLACE;
        case SO_SaturatedIncrement: return D3D11_STENCIL_OP_INCR_SAT;
        case SO_SaturatedDecrement: return D3D11_STENCIL_OP_DECR_SAT;
        case SO_Invert: return D3D11_STENCIL_OP_INVERT;
        case SO_Increment: return D3D11_STENCIL_OP_INCR;
        case SO_Decrement: return D3D11_STENCIL_OP_DECR;
        default: return D3D11_STENCIL_OP_KEEP;
        };
    }
    
    static D3D11_BLEND_OP TranslateBlendOp(EBlendOperation BlendOp)
    {
        switch(BlendOp)
        {
        case BO_Subtract: return D3D11_BLEND_OP_SUBTRACT;
        case BO_Min: return D3D11_BLEND_OP_MIN;
        case BO_Max: return D3D11_BLEND_OP_MAX;
        case BO_ReverseSubtract: return D3D11_BLEND_OP_REV_SUBTRACT;
        default: return D3D11_BLEND_OP_ADD;
        };
    }
    
    static D3D11_BLEND TranslateBlendFactor(EBlendFactor BlendFactor)
    {
        switch(BlendFactor)
        {
        case BF_One: return D3D11_BLEND_ONE;
        case BF_SourceColor: return D3D11_BLEND_SRC_COLOR;
        case BF_InverseSourceColor: return D3D11_BLEND_INV_SRC_COLOR;
        case BF_SourceAlpha: return D3D11_BLEND_SRC_ALPHA;
        case BF_InverseSourceAlpha: return D3D11_BLEND_INV_SRC_ALPHA;
        case BF_DestAlpha: return D3D11_BLEND_DEST_ALPHA;
        case BF_InverseDestAlpha: return D3D11_BLEND_INV_DEST_ALPHA;
        case BF_DestColor: return D3D11_BLEND_DEST_COLOR;
        case BF_InverseDestColor: return D3D11_BLEND_INV_DEST_COLOR;
        case BF_ConstantBlendFactor: return D3D11_BLEND_BLEND_FACTOR;
        case BF_InverseConstantBlendFactor: return D3D11_BLEND_INV_BLEND_FACTOR;
        case BF_Source1Color: return D3D11_BLEND_SRC1_COLOR;
        case BF_InverseSource1Color: return D3D11_BLEND_INV_SRC1_COLOR;
        case BF_Source1Alpha: return D3D11_BLEND_SRC1_ALPHA;
        case BF_InverseSource1Alpha: return D3D11_BLEND_INV_SRC1_ALPHA;
	
        default: return D3D11_BLEND_ZERO;
        };
    }
    
    static D3D11_CULL_MODE TranslateCullMode(ERasterizerCullMode CullMode)
    {
        switch(CullMode)
        {
        case RCM_Back: return D3D11_CULL_BACK;
        case RCM_Front: return D3D11_CULL_FRONT;
        default: return D3D11_CULL_NONE;
        };
    }
    
    static D3D11_FILL_MODE TranslateFillMode(ERasterizerFillMode FillMode)
    {
        switch(FillMode)
        {
        case RFM_Wireframe: return D3D11_FILL_WIREFRAME;
        default: return D3D11_FILL_SOLID;
        };
    }

	static D3D11_TEXTURE_ADDRESS_MODE TranslateAddressMode(ESamplerAddressMode AddressMode)
    {
    	switch(AddressMode)
    	{
    	case AM_Clamp: return D3D11_TEXTURE_ADDRESS_CLAMP;
    	case AM_Mirror: return D3D11_TEXTURE_ADDRESS_MIRROR;
    	case AM_Border: return D3D11_TEXTURE_ADDRESS_BORDER;
    	default: return D3D11_TEXTURE_ADDRESS_WRAP;
    	};
    }

	uint32_t ComputeAnisotropyRT(int32_t InitializerMaxAnisotropy)
    {
	    return std::clamp(InitializerMaxAnisotropy, 1, 16);
    }

	static D3D11_COMPARISON_FUNC TranslateSamplerCompareFunction(ESamplerCompareFunction SamplerComparisonFunction)
    {
    	switch(SamplerComparisonFunction)
    	{
    	case SCF_Less: return D3D11_COMPARISON_LESS;
    	case SCF_Never: 
    	default: return D3D11_COMPARISON_NEVER;
    	};
    }

	
	
    Ref<RHIDepthStencilState> D3D11DynamicRHI::RHICreateDepthStencilState(const DepthStencilStateInitializer& initializer)
    {
        
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
        // depth part
        depthStencilDesc.DepthEnable = initializer.DepthTest != CF_Always || initializer.bEnableDepthWrite;
        depthStencilDesc.DepthWriteMask = initializer.bEnableDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        depthStencilDesc.DepthFunc = TranslateCompareFunction(initializer.DepthTest);
        
        // stencil part
        depthStencilDesc.StencilEnable = initializer.bEnableFrontFaceStencil || initializer.bEnableBackFaceStencil;
        depthStencilDesc.StencilReadMask = initializer.StencilReadMask;
        depthStencilDesc.StencilWriteMask = initializer.StencilWriteMask;
        depthStencilDesc.FrontFace.StencilFunc = TranslateCompareFunction(initializer.FrontFaceStencilTest);
        depthStencilDesc.FrontFace.StencilFailOp = TranslateStencilOp(initializer.FrontFaceStencilFailStencilOp);
        depthStencilDesc.FrontFace.StencilDepthFailOp = TranslateStencilOp(initializer.FrontFaceDepthFailStencilOp);
        depthStencilDesc.FrontFace.StencilPassOp = TranslateStencilOp(initializer.FrontFacePassStencilOp);
        if( initializer.bEnableBackFaceStencil )
        {
            depthStencilDesc.BackFace.StencilFunc = TranslateCompareFunction(initializer.BackFaceStencilTest);
            depthStencilDesc.BackFace.StencilFailOp = TranslateStencilOp(initializer.BackFaceStencilFailStencilOp);
            depthStencilDesc.BackFace.StencilDepthFailOp = TranslateStencilOp(initializer.BackFaceDepthFailStencilOp);
            depthStencilDesc.BackFace.StencilPassOp = TranslateStencilOp(initializer.BackFacePassStencilOp);
        }
        else
        {
            depthStencilDesc.BackFace = depthStencilDesc.FrontFace;
        }
        const bool bStencilOpIsKeep =
           initializer.FrontFaceStencilFailStencilOp == SO_Keep
        && initializer.FrontFaceDepthFailStencilOp == SO_Keep
        && initializer.FrontFacePassStencilOp == SO_Keep
        && initializer.BackFaceStencilFailStencilOp == SO_Keep
        && initializer.BackFaceDepthFailStencilOp == SO_Keep
        && initializer.BackFacePassStencilOp == SO_Keep;
	    const bool bMayWriteStencil = initializer.StencilWriteMask != 0 && !bStencilOpIsKeep;
		
		ID3D11DepthStencilState* StateHandle = nullptr;
		if (FAILED(this->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &StateHandle)))
		{
			LEMON_CORE_ERROR("Failed Create D3D11DepthStencilState");
			return nullptr;
		}
		const auto result =  GDepthStencilStateCache.find(StateHandle);
		if(result != GDepthStencilStateCache.end())
		{
			return result->second;
		}
		
		Ref<RHIDepthStencilState> depthStencilState = CreateRef<D3D11DepthStencilState>(initializer);
        D3D11DepthStencilState* d3d11DepthStencilState = RefCast<D3D11DepthStencilState>(depthStencilState);
        d3d11DepthStencilState->AccessType.SetDepthStencilWrite(initializer.bEnableDepthWrite, bMayWriteStencil);
		d3d11DepthStencilState->m_Resource = StateHandle;
		// Store
		GDepthStencilStateCache[StateHandle] = depthStencilState;
		
        return depthStencilState;
    }
    
    Ref<RHIBlendState> D3D11DynamicRHI::RHICreateBlendState(const BlendStateInitializer& initializer)
    {
        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.AlphaToCoverageEnable = false;
        blendDesc.IndependentBlendEnable = initializer.bUseIndependentRenderTargetBlendStates;
        
        static_assert(MaxSimultaneousRenderTargets <= D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, "Too many MRTs.");

        for(uint32_t RenderTargetIndex = 0;RenderTargetIndex < MaxSimultaneousRenderTargets;++RenderTargetIndex)
        {
            const BlendStateInitializer::RenderTargetBlendState RenderTargetInitializer = initializer.RenderTargets[RenderTargetIndex];
            D3D11_RENDER_TARGET_BLEND_DESC& RenderTarget = blendDesc.RenderTarget[RenderTargetIndex];
            RenderTarget.BlendEnable = 
                RenderTargetInitializer.ColorBlendOp != BO_Add || RenderTargetInitializer.ColorDestBlend != BF_Zero || RenderTargetInitializer.ColorSrcBlend != BF_One ||
                RenderTargetInitializer.AlphaBlendOp != BO_Add || RenderTargetInitializer.AlphaDestBlend != BF_Zero || RenderTargetInitializer.AlphaSrcBlend != BF_One;
            RenderTarget.BlendOp = TranslateBlendOp(RenderTargetInitializer.ColorBlendOp);
            RenderTarget.SrcBlend = TranslateBlendFactor(RenderTargetInitializer.ColorSrcBlend);
            RenderTarget.DestBlend = TranslateBlendFactor(RenderTargetInitializer.ColorDestBlend);
            RenderTarget.BlendOpAlpha = TranslateBlendOp(RenderTargetInitializer.AlphaBlendOp);
            RenderTarget.SrcBlendAlpha = TranslateBlendFactor(RenderTargetInitializer.AlphaSrcBlend);
            RenderTarget.DestBlendAlpha = TranslateBlendFactor(RenderTargetInitializer.AlphaDestBlend);
            RenderTarget.RenderTargetWriteMask = 
              ((RenderTargetInitializer.ColorWriteMask & CW_RED)   ? D3D11_COLOR_WRITE_ENABLE_RED   : 0)
            | ((RenderTargetInitializer.ColorWriteMask & CW_GREEN) ? D3D11_COLOR_WRITE_ENABLE_GREEN : 0)
            | ((RenderTargetInitializer.ColorWriteMask & CW_BLUE)  ? D3D11_COLOR_WRITE_ENABLE_BLUE  : 0)
            | ((RenderTargetInitializer.ColorWriteMask & CW_ALPHA) ? D3D11_COLOR_WRITE_ENABLE_ALPHA : 0);
        }
		//GBlendStateCache
		ID3D11BlendState* StateHandle = nullptr;
		if (FAILED(this->GetDevice()->CreateBlendState(&blendDesc, &StateHandle)))
		{
			LEMON_CORE_ERROR("Failed Create D3D11BlendState");
			return nullptr;
		}
		const auto result =  GBlendStateCache.find(StateHandle);
		if(result != GBlendStateCache.end())
		{
			return result->second;
		}
		
        Ref<RHIBlendState> BlendState = CreateRef<D3D11BlendState>(initializer);
        D3D11BlendState* d3d11BlendState = RefCast<D3D11BlendState>(BlendState);

		// store
		d3d11BlendState->m_Resource = StateHandle;
		
        return BlendState;
    }
    
    Ref<RHIRasterizerState> D3D11DynamicRHI::RHICreateRasterizerState(const RasterizerStateInitializer& initializer)
    {
        D3D11_RASTERIZER_DESC rasterizerDesc = {};

        rasterizerDesc.CullMode = TranslateCullMode(initializer.CullMode);
        rasterizerDesc.FillMode = TranslateFillMode(initializer.FillMode);
        rasterizerDesc.FrontCounterClockwise = true;
        rasterizerDesc.DepthBias = 0;//FMath::FloorToInt(Initializer.DepthBias * (float)(1 << 24));
        rasterizerDesc.SlopeScaledDepthBias = 0;//initializer.SlopeScaleDepthBias;
		rasterizerDesc.DepthBiasClamp = 0.0f;
        rasterizerDesc.DepthClipEnable = true;
        rasterizerDesc.MultisampleEnable = false;//Initializer.bAllowMSAA;
        rasterizerDesc.ScissorEnable = false;
		
		// D3D11 will return the same pointer if the particular state description was already created
		ID3D11RasterizerState* StateHandle = nullptr;
		if (FAILED(this->GetDevice()->CreateRasterizerState(&rasterizerDesc, &StateHandle)))
		{
			LEMON_CORE_ERROR("Failed Create D3D11RasterizerState");
			return nullptr;
		}
		const auto result =  GRasterizerStataCache.find(StateHandle);
		if(result != GRasterizerStataCache.end())
		{
			return result->second;
		}
		
        Ref<RHIRasterizerState> rasterizerState = CreateRef<D3D11RasterizerState>(initializer);
        D3D11RasterizerState* d3d11RasterizerState = RefCast<D3D11RasterizerState>(rasterizerState);
		d3d11RasterizerState->m_Resource = StateHandle;
		GRasterizerStataCache[StateHandle] = rasterizerState;
		return rasterizerState;
    }
	
	
    Ref<RHISamplerState> D3D11DynamicRHI::RHICreateSamplerState(const SamplerStateInitializer& initializer)
    {
        D3D11_SAMPLER_DESC samplerDesc = {};

		samplerDesc.AddressU = TranslateAddressMode(initializer.AddressU);
		samplerDesc.AddressV = TranslateAddressMode(initializer.AddressV);
		samplerDesc.AddressW = TranslateAddressMode(initializer.AddressW);
		samplerDesc.MipLODBias = initializer.MipBias;
		samplerDesc.MaxAnisotropy = ComputeAnisotropyRT(initializer.MaxAnisotropy);
		samplerDesc.MinLOD = initializer.MinMipLevel;
		samplerDesc.MaxLOD = initializer.MaxMipLevel;

		// Determine whether we should use one of the comparison modes
		const bool bComparisonEnabled = initializer.SamplerComparisonFunction != SCF_Never;
		switch(initializer.Filter)
		{
		case SF_AnisotropicLinear:
		case SF_AnisotropicPoint:
			if (samplerDesc.MaxAnisotropy == 1)
			{
				samplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
			else
			{
				// D3D11 doesn't allow using point filtering for mip filter when using anisotropic filtering
				samplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_ANISOTROPIC : D3D11_FILTER_ANISOTROPIC;
			}
			break;
		case SF_Trilinear:
	        samplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case SF_Bilinear:
			samplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT : D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case SF_Point:
			samplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT : D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		}
		samplerDesc.BorderColor[0] = initializer.BorderColor.r;
		samplerDesc.BorderColor[1] = initializer.BorderColor.g;
		samplerDesc.BorderColor[2] = initializer.BorderColor.b;
		samplerDesc.BorderColor[3] = initializer.BorderColor.a;
		samplerDesc.ComparisonFunc = TranslateSamplerCompareFunction(initializer.SamplerComparisonFunction);

		// D3D11 will return the same pointer if the particular state description was already created
		ID3D11SamplerState* SamplerStateHandle = nullptr;
		if (FAILED(this->GetDevice()->CreateSamplerState(&samplerDesc, &SamplerStateHandle)))
		{
			LEMON_CORE_ERROR("Failed Create D3D11SamplerState");
			return nullptr;
		}
		const auto result =  GSamplerStateCache.find(SamplerStateHandle);
		if(result != GSamplerStateCache.end())
		{
			return result->second;
		}
		
		Ref<RHISamplerState> samplerState = CreateRef<D3D11SamplerState>(initializer);
		D3D11SamplerState* d3d11samplerState = RefCast<D3D11SamplerState>(samplerState);
		d3d11samplerState->m_Resource = SamplerStateHandle;
		GSamplerStateCache[SamplerStateHandle] = samplerState;
		return samplerState;
	}
}