#include "Common.hlsl"

struct VertexOutput
{
	float4 Position 		: SV_POSITION;
	float4 WorldPosition 	: POSITION0;
	float4 Color 			: COLOR;
	float3 Normal			: NORMAL;
	float3 Direction 		: COLOR1;
};

VertexOutput MainVS(VertexInput Input)
{
	VertexOutput Output;
	float4 LocalPos = float4(Input.Position, 1.0f);
	float4 WorldPos = mul(g_LocalToWorldMatrix, LocalPos); 
    Output.Position = mul(g_ViewProjectionMatrix, float4(WorldPos.xyz, 1.0f));
	Output.Position.z = Output.Position.w  * 0.9999;//0.9999;// set max distance
	//float4 ViewPos = mul(g_ViewMatrix, WorldPos);
	//Output.Position = mul(g_ProjectionMatrix, ViewPos);
	Output.WorldPosition = WorldPos;
	//Output.Position = mul float4(Input.Position, 1.0f);
	Output.Color = LocalPos; //Input.Color;
	float4 Normal = float4(LocalPos.xyz, 1.0f);
	Output.Normal = mul(g_WorldToLocalTransposeMatrix, Normal).xyz;
	Output.Direction = normalize(LocalPos.xyz);
	return Output;
}