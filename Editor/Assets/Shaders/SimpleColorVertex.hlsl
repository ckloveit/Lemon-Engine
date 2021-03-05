
struct VertexInput
{
	float3 Position : ATTRIBUTE0;
	float4 Color : ATTRIBUTE1;
};

struct VertexOutput
{
	float4 Position : SV_POSITION;
	float4 Color 	: COLOR;
};
VertexOutput MainVS(VertexInput Input)
{
	VertexOutput Output;
	Output.Position = float4(Input.Position, 1.0f);
	Output.Color = Input.Color;
	return Output;
}