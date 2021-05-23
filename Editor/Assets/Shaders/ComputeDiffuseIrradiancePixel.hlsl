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
	float3 Dir = normalize(Input.WorldPosition.xyz);
	float3 Normal = normalize(Input.WorldPosition.xyz);
	float3 Irradiance = 0;

	float3 Up = float3(0.0f, 1.0f, 0.0f);
	float3 Right = cross(Up, Normal);
	Up = cross(Normal, Right);

	float SampleDelta = 0.025f;
	float nSamples = 0.0f;

	for (float Phi = 0.0f; Phi < 2.0f * PI * 0.1; Phi += SampleDelta)
	{
		for (float Theta = 0.0f; Theta < 0.5f * PI * 0.1; Theta += SampleDelta)
		{
			// Spherical to cartesian 
			float3 TangentSample = float3(sin(Theta) * cos(Phi), sin(Theta) * sin(Phi), cos(Theta));

			// Tangent to world
			float3 SampleVec = TangentSample.x * Right + TangentSample.y * Up + TangentSample.z * Normal;

			Irradiance += EnvironmentTexCube.SampleLevel(BilinearClampedSampler, SampleVec, 0).rgb * cos(Theta) * sin(Theta);
			
			nSamples += 1.0f;
		}
	}
	Irradiance = PI * Irradiance * (1.0f / nSamples);
	//return EnvironmentTexCube.SampleLevel(BilinearClampedSampler, Dir, 0);
	return float4(Irradiance, 1.0f);
}