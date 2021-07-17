#include "Common.hlsl"
TextureCube EnvironmentTexCube           : register(t0);

struct PixelInput
{
	float4 Position 		: SV_POSITION;
	float4 WorldPosition 	: POSITION0;
	float4 Color 			: COLOR;
	float3 Normal			: NORMAL;
	float3 Direction 		: COLOR1;
};

struct PixelOutput
{
    float4 PositionWS   : SV_Target0;
    float4 Albedo   : SV_Target1;
    float4 Normal   : SV_Target2;
    float4 Material : SV_Target3;
};

PixelOutput MainPS(PixelInput Input)
{
 	PixelOutput Output;
    Output.PositionWS           = Input.WorldPosition;
    Output.Albedo 				= EnvironmentTexCube.SampleLevel(TrilinearWrapSampler, Input.Direction, 0);;
    Output.Normal  				= float4(Input.Normal, 1.0f);
    Output.Material				= g_PBRParameters;
    
    return Output;
}