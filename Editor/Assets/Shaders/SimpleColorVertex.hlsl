#include "Common.hlsl"

struct VertexOutput
{
	float4 Position : SV_POSITION;
	float4 Color 	: COLOR;
};

VertexOutput MainVS(VertexInput Input)
{
	VertexOutput Output;
	float4 LocalPos = float4(Input.Position, 1.0f);
	float4 WorldPos = mul(g_LocalToWorldMatrix, LocalPos);
	Output.Position = mul(g_ViewProjectionMatrix, WorldPos);
	Output.Color = Input.Color;
	return Output;
}