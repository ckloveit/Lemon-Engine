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
	 return EnvironmentTexCube.SampleLevel(BilinearClampedSampler, Input.Dir, 0);
}