#include "Common.hlsl"
#define MAX_MESH_TEXTURE_COORDS 2
struct VertexInput
{
	float3 Position : ATTRIBUTE0;
	float4 Color : ATTRIBUTE1;
	float3 Normal : ATTRIBUTE2;
	float3 Tangent : ATTRIBUTE3;
	float2 Texcoords[MAX_MESH_TEXTURE_COORDS] : ATTRIBUTE4;
};

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
	//Output.Position = mul float4(Input.Position, 1.0f);
	Output.Color = Output.Position;//Input.Color;
	return Output;
}