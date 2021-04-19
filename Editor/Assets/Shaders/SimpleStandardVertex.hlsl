#include "Common.hlsl"

struct VertexOutput
{
	float4 Position : SV_POSITION;
	float4 Color 	: COLOR;
	float3 Normal	: NORMAL;
};

VertexOutput MainVS(VertexInput Input)
{
	VertexOutput Output;
	float4 LocalPos = float4(Input.Position, 1.0f);
	float4 WorldPos = mul(g_LocalToWorldMatrix, LocalPos);
    Output.Position = mul(g_ViewProjectionMatrix, WorldPos);
	//Output.Position = mul float4(Input.Position, 1.0f);
	Output.Color = LocalPos; //Input.Color;
	float4 Normal = float4(LocalPos.xyz, 1.0f);
	Output.Normal = mul(g_WorldToWorldTransposeMatrix, Normal).xyz;
	return Output;
}