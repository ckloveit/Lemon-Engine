#include "Common.hlsl"
TextureCube EnvironmentTexCube           : register(t0);

struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Color 	: COLOR;
	float2 UV0	: TEXCOORD0;
};

float4 MainPS(PixelInput Input) : SV_TARGET
{
	float roughness = g_CustomDataFloat4_CustomData0.x;
	float3 WorldPos = Input.WorldPosition.xyz;
	float3 N = normalize(WorldPos);
	float3 R = N;
	float3 V = R;

	const uint SAMPLE_COUNT = 1024u;
	float totalWeight = 0.0;
	float3 prefilteredColor = float3(0.0, 0.0, 0.0);
	for (uint i = 0u; i < SAMPLE_COUNT; ++i)
	{
		float2 Xi = Hammersley(i, SAMPLE_COUNT);
		float3 H = ImportanceSampleGGX(Xi, N, roughness);
		float3 L = normalize(2.0 * dot(V, H) * H - V);

		float NdotL = max(dot(N, L), 0.0);
		if (NdotL > 0.0)
		{
			prefilteredColor += EnvironmentTexCube.Sample(TrilinearWrapSampler, L).rgb * NdotL;
			totalWeight += NdotL;
		}
	}
	prefilteredColor = prefilteredColor / totalWeight;

	return float4(prefilteredColor, 1.0);
}