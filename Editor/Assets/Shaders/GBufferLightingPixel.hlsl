#include "Common.hlsl"
Texture2D GBufferPositionTex           : register(t0);
Texture2D GBufferAlbedoTex             : register(t1);
Texture2D GBufferNormalTex             : register(t2);
Texture2D GBufferMaterialTex           : register(t3);

struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Color 	: COLOR;
	float2 UV0	: TEXCOORD0;
};

float4 MainPS(PixelInput Input) : SV_TARGET
{
	//float3 SourceColor = GBufferNormalTex.Sample(BilinearClampedSampler, Input.UV0).rgb;
	float3 SourceColor = GBufferAlbedoTex.Sample(BilinearClampedSampler, Input.UV0).rgb;
	return float4(SourceColor, 1.0);
}