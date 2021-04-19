#include "Common.hlsl"

struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 Color 	: COLOR;
	float3 Normal	: NORMAL;
};

float4 MainPS(PixelInput Input) : SV_TARGET
{
	[branch]
	if(g_DirectionalLightDir.w > 0.0f)
	{
		float3 L = g_DirectionalLightDir.xyz;
		float3 N = Input.Normal;
		float NDotL = dot(N, L);
		return float4(g_DirectionalLightColor.rgb * NDotL, 1.0f);
	}
	return Input.Color;
	//return float4(Input.Normal, 1.0f);//float4(g_TestColor, 1.0f);//
}