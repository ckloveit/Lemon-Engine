#include "Common.hlsl"
TextureCube EnvironmentTexCube           : register(t0);
struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 Color 	: COLOR;
	float3 Dir		: COLOR1;
};

float4 MainPS(PixelInput Input) : SV_TARGET
{
	float roughness = g_PBRParameters.y;
	float mipLvel = 0;//roughness * 5.0f;
	return EnvironmentTexCube.SampleLevel(TrilinearWrapSampler, Input.Dir, mipLvel);
}