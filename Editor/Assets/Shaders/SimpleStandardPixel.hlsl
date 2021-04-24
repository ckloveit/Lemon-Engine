#include "Common.hlsl"

struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Color 	: COLOR;
	float3 Normal	: NORMAL;
};

float4 MainPS(PixelInput Input) : SV_TARGET
{
	float roughness = g_PBRParameters.y;
	float metallic = g_PBRParameters.x;
	float ao = g_PBRParameters.z;

	float3 Albedo = g_Albedo.xyz;

	float3 Lo = 0;
	float3 F0 = float3(0.04f, 0.04f, 0.04f);
	F0 = lerp(F0, Albedo, metallic);
	[branch]
	if(g_DirectionalLightDir.w > 0.0f)
	{
		float3 L = g_DirectionalLightDir.xyz;
		float3 N = normalize(Input.Normal);
		float3 V = normalize(g_CameraWorldPosition.xyz - Input.WorldPosition.xyz);
		float3 H = normalize(V + L);

		float attenuation = 1.0f;//DirectionLight has no transport attenuation
		float3 radiance = g_DirectionalLightColor.rgb * attenuation;// Get the L(p, wi)

		// Cook-Torrance BRDF
		float NDF = D_GGX(N, H, roughness);
		float G = Vis_GeometrySmith(N, V, L, roughness);
		float3 F = Fresnel_Schlick(max(dot(H, V), 0.0), F0);

		float3 KS = F;
		float3 KD = float3(1.0f, 1.0f, 1.0f) - KS;
		KD *= 1.0f - metallic;
		
		float3 nominator = NDF * G * F;
		float denominator = 4.0f * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		float3 specular = nominator / denominator;

		// combine
		float NDotL = max(dot(N, L), 0.0f);
		Lo += (KD * Albedo / PI + specular) * radiance * NDotL;
	}

	float3 ambient = float3(0.03, 0.03, 0.03) * Albedo * ao;
	float3 color = ambient + Lo;
	
	// Tonemap
	color = color / (color + float3(1.0f, 1.0f, 1.0f));
	
	// Gamma Inverse Correct
	float invGamma = 1.0f / 2.2f;
	color = pow(color, float3(invGamma, invGamma, invGamma));

	return float4(color, 1.0f);
	//return float4(Input.Normal, 1.0f);//float4(g_TestColor, 1.0f);//
}