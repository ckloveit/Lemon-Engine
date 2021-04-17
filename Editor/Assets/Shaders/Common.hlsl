#include "CommonBuffer.hlsl"
#include "CommonSampler.hlsl"

#define MAX_MESH_TEXTURE_COORDS 2
struct VertexInput
{
	float3 Position : ATTRIBUTE0;
	float4 Color : ATTRIBUTE1;
	float3 Normal : ATTRIBUTE2;
	float3 Tangent : ATTRIBUTE3;
	float2 Texcoords[MAX_MESH_TEXTURE_COORDS] : ATTRIBUTE4;
};