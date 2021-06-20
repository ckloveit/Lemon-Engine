#include "Common.hlsl"
Texture2D InputTexture0           : register(t0);

struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Color 	: COLOR;
	float2 UV0	: TEXCOORD0;
};

float4 MainPS(PixelInput Input) : SV_TARGET
{
	float3 SourceColor = InputTexture0.Sample(BilinearClampedSampler, Input.UV0).rgb;
	return float4(SourceColor, 1.0);
}