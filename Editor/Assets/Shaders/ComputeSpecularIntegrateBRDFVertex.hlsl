#include "Common.hlsl"

struct VertexOutput
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Color 	: COLOR;
	float2 UV0	: TEXCOORD0;
};

VertexOutput MainVS(VertexInput Input)
{
	VertexOutput Output;
	float4 LocalPos = float4(Input.Position, 1.0f);
    Output.Position = LocalPos;
	Output.WorldPosition = mul(g_FullScreen_LocalToWorldMatrix, float4(Input.Position.xy, 1.0f, 1.0f));
	Output.Color = LocalPos; //Input.Color;
	Output.UV0 = Input.Texcoords[0];
	return Output;
}