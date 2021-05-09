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
	Output.WorldPosition = LocalPos;
	//Output.Position = mul float4(Input.Position, 1.0f);
	Output.Color = LocalPos; //Input.Color;
	Output.UV0 = LocalPos.xy;
	return Output;
}