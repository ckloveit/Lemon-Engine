#include "Common.hlsl"

struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Color 	: COLOR;
	float2 UV0	: TEXCOORD0;
};

float4 MainPS(PixelInput Input) : SV_TARGET
{
	return float4(Input.UV0, 0.0f, 1.0f);
}