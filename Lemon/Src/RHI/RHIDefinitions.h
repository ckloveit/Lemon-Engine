#pragma once
#include "Core/Core.h"
#include "RenderCore/Containers/ResourceArray.h"
#include "RHI.h"
#include <map>
#include <array>

namespace Lemon
{
	enum
	{
		MaxVertexElementCount = 16,
		MaxVertexElementCount_NumBits = 4,
		MaxSimultaneousRenderTargets = 8,
	};
	
	enum ERHIVertexElementType
	{
		VET_None,
		VET_Float1,
		VET_Float2,
		VET_Float3,
		VET_Float4,
		VET_PackedNormal,	// FPackedNormal
		VET_UByte4,
		VET_UByte4N,
		VET_Color,
		VET_Short2,
		VET_Short4,
		VET_Short2N,		// 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
		VET_Half2,			// 16 bit float using 1 bit sign, 5 bit exponent, 10 bit mantissa 
		VET_Half4,
		VET_Short4N,		// 4 X 16 bit word, normalized 
		VET_UShort2,
		VET_UShort4,
		VET_UShort2N,		// 16 bit word normalized to (value/65535.0,value/65535.0,0,0,1)
		VET_UShort4N,		// 4 X 16 bit word unsigned, normalized 
		VET_URGB10A2N,		// 10 bit r, g, b and 2 bit a normalized to (value/1023.0f, value/1023.0f, value/1023.0f, value/3.0f)
		VET_UInt,
		VET_MAX,

		VET_NumBits = 5,
	};

	enum EPrimitiveType
	{
		// Topology that defines a triangle N with 3 vertex extremities: 3*N+0, 3*N+1, 3*N+2.
		PT_TriangleList,
		PT_LineList,
	};


	///RHI Data Type
	enum ERHIPixelFormat : uint32_t // gets serialized so better be explicit
	{
		// R
		RHI_PF_R8_Unorm,
		RHI_PF_R16_Uint,
		RHI_PF_R16_Float,
		RHI_PF_R32_Uint,
		RHI_PF_R32_Float,
		// RG
		RHI_PF_R8G8_Unorm,
		RHI_PF_R16G16_Float,
		RHI_PF_R32G32_Float,
		// RGB
		RHI_PF_R11G11B10_Float,
		RHI_PF_R32G32B32_Float,
		// RGBA
		RHI_PF_R8G8B8A8_Unorm,
		RHI_PF_R10G10B10A2_Unorm,
		RHI_PF_R16G16B16A16_Snorm,
		RHI_PF_R16G16B16A16_Float,
		RHI_PF_R32G32B32A32_Float,
		// DEPTH
		RHI_PF_D32_Float,
		RHI_PF_D32_Float_S8X24_Uint,
		// UNKNOWN
		RHI_PF_Undefined,
		//
		RHI_PF_MAX
	};

	enum ERHITextureCreateFlags
	{
		RHI_TexCreate_None = 0,
		// Texture can be used as a render target
		RHI_TexCreate_RenderTargetable = 1 << 0,
		// Texture can be used as a depth-stencil target.
		RHI_TexCreate_DepthStencilTargetable = 1 << 1,
		// Texture can be used as a shader resource.
		RHI_TexCreate_ShaderResource = 1 << 2,
		// UnorderedAccessView (DX11 only)
		RHI_TexCreate_UnorderedAccessView = 1 << 3,


	};
	/**
	 *	Resource usage flags - for vertex and index buffers.
	 */
	enum EBufferUsageFlags
	{
		BUF_None = 0,
		// The buffer will be written to once.
		BUF_Static = 1 << 0,
		// The buffer will be written to occasionally, GPU read only, CPU write only.  The data lifetime is until the next update, or the buffer is destroyed.
		BUF_Dynamic = 1 << 1,


	};

	enum EShaderFrequency : uint8_t
	{
		SF_Vertex = 0,
		SF_Hull = 1,
		SF_Domain = 2,
		SF_Pixel = 3,
		SF_Geometry = 4,
		SF_Compute = 5,
	};

	enum EUniformBufferUsageScope : uint8_t
	{
		UBUS_Vertex = 1 << 0,
		UBUS_Pixel = 1 << 1,
	};
	using EUniformBufferUsageScopeType = uint8_t;

	enum ERasterizerFillMode
	{
		RFM_Wireframe,
		RFM_Solid,
	};

	// RCM_CCW is FRONT face
	// RCM_CW is back face
	enum ERasterizerCullMode
	{
		RCM_None,
		RCM_Front,
		RCM_Back,
	};

	enum ECompareFunction
	{
		CF_Less,
		CF_LessEqual,
		CF_Greater,
		CF_GreaterEqual,
		CF_Equal,
		CF_NotEqual,
		CF_Never,
		CF_Always,
	};
	enum EStencilOp
	{
		SO_Keep,
    	SO_Zero,
    	SO_Replace,
    	SO_SaturatedIncrement,
    	SO_SaturatedDecrement,
    	SO_Invert,
    	SO_Increment,
    	SO_Decrement,
	};

	enum EBlendOperation
	{
		BO_Add,
        BO_Subtract,
        BO_Min,
        BO_Max,
        BO_ReverseSubtract,
	};

	enum EBlendFactor
	{
		BF_Zero,
        BF_One,
        BF_SourceColor,
        BF_InverseSourceColor,
        BF_SourceAlpha,
        BF_InverseSourceAlpha,
        BF_DestAlpha,
        BF_InverseDestAlpha,
        BF_DestColor,
        BF_InverseDestColor,
        BF_ConstantBlendFactor,
        BF_InverseConstantBlendFactor,
        BF_Source1Color,
        BF_InverseSource1Color,
        BF_Source1Alpha,
        BF_InverseSource1Alpha,
    };

	enum EColorWriteMask
	{
		CW_RED   = 0x01,
	    CW_GREEN = 0x02,
	    CW_BLUE  = 0x04,
	    CW_ALPHA = 0x08,

	    CW_NONE  = 0,
	    CW_RGB   = CW_RED | CW_GREEN | CW_BLUE,
	    CW_RGBA  = CW_RED | CW_GREEN | CW_BLUE | CW_ALPHA,
	    CW_RG    = CW_RED | CW_GREEN,
	    CW_BA    = CW_BLUE | CW_ALPHA,
	};

	enum ESamplerFilter
	{
		SF_Point,
	    SF_Bilinear,
	    SF_Trilinear,
	    SF_AnisotropicPoint,
	    SF_AnisotropicLinear,
	};

	enum ESamplerAddressMode
	{
		AM_Wrap,
        AM_Clamp,
        AM_Mirror,
        /** Not supported on all platforms */
        AM_Border,
    };

	enum ESamplerCompareFunction
	{
		SCF_Never,
        SCF_Less
    };
	
}