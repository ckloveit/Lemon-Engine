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
	/*
	float3 Dir = normalize(Input.WorldPosition.xyz);
	float3 Normal = normalize(Input.WorldPosition.xyz);
	float3 Irradiance = 0;

	float3 Up = float3(0.0f, 1.0f, 0.0f);
	float3 Right = normalize(cross(Up, Normal));
	Up = normalize(cross(Normal, Right));

	float SampleDelta = 0.025f;
	float nSamples = 0.0f;

	for (float Phi = 0.0f; Phi < 2.0f * PI; Phi += SampleDelta)
	{
		for (float Theta = 0.0f; Theta < 0.5f * PI; Theta += SampleDelta)
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
	*/
	// The world vector acts as the normal of a tangent surface
	// from the origin, aligned to WorldPos. Given this normal, calculate all
	// incoming radiance of the environment. The result of this radiance
	// is the radiance of light coming from -Normal direction, which is what
	// we use in the PBR shader to sample irradiance.
	float3 WorldPos = Input.WorldPosition.xyz;
	float3 N = normalize(WorldPos);

	float3 irradiance = float3(0.0, 0.0, 0.0);

	// tangent space calculation from origin point
	float3 up = float3(0.0, 1.0, 0.0);
	float3 right = normalize(cross(up, N));
	up = normalize(cross(N, right));

	float sampleDelta = 0.025;
	float nrSamples = 0.0;
	for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			// spherical to cartesian (in tangent space)
			float3 tangentSample = float3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
			// tangent space to world
			float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

			irradiance += EnvironmentTexCube.Sample(BilinearClampedSampler, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));

	return float4(irradiance, 1.0);
}