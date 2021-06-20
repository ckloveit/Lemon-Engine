#include "Common.hlsl"
Texture2D EquirectangularSkyMap           : register(t0);

struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Color 	: COLOR;
	float2 UV0	: TEXCOORD0;
};

static const float2 invAtan = float2(0.1591, 0.3183);
float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

float4 MainPS(PixelInput Input) : SV_TARGET
{
	float2 uv = SampleSphericalMap(normalize(Input.WorldPosition.xyz));
    float3 colour = EquirectangularSkyMap.Sample(BilinearWrapSampler, uv).rgb;
		
    return float4(colour, 1.0);
}