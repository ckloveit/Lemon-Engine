#include "Common.hlsl"

//--------------------IBL--------------------//
TextureCube diffuseIrradianceMap : register(t0);
TextureCube specularPrefilterMap : register(t1);
Texture2D specularBRDFLutMap : register(t2);
//-------------------------------------------//

Texture2D GBufferPositionTex           : register(t3);
Texture2D GBufferAlbedoTex             : register(t4);
Texture2D GBufferNormalTex             : register(t5);
Texture2D GBufferMaterialTex           : register(t6);
Texture2D SceneDepthTex           	   : register(t7);

struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Color 	: COLOR;
	float2 UV0	: TEXCOORD0;
};


float3 ComputeWorldPosition(float2 UV, float SceneDepth)
{
	float2 UVClip = UV * 2.0f - float2(1.0f, 1.0f);
	float4 ClipPos = float4(UVClip, SceneDepth, 1.0);

	// Inverse projection by clip position
	float4 ViewPos = mul(g_InverseProjectionMatrix, ClipPos);

	// perspective division
	ViewPos /= ViewPos.w;

	// View to WorldPos

	float3 WorldPos = mul(g_InverseViewMatrix, ViewPos).xyz;

	return WorldPos;
}



float4 MainPS(PixelInput Input) : SV_TARGET
{
	//Debug
	float roughness = 0;
	float metallic = 0.0f;
 	float ao = 1.0;
	float3 albedo = float3(1.0f, 0.0f, 0.0f);


	float3 Lo = 0;
	float3 F0 = float3(0.04f, 0.04f, 0.04f);
	F0 = lerp(F0, albedo, metallic);

	//float3 SourceColor = GBufferNormalTex.Sample(BilinearClampedSampler, Input.UV0).rgb;
	//float3 SourceColor = GBufferAlbedoTex.Sample(BilinearClampedSampler, Input.UV0).rgb;

	float3 WorldPosition = GBufferPositionTex.Sample(BilinearClampedSampler, Input.UV0).rgb;
	float SceneDepth = SceneDepthTex.Sample(PointClampedSampler, Input.UV0).r;

	WorldPosition = ComputeWorldPosition(Input.UV0, SceneDepth);

	float3 N = GBufferNormalTex.Sample(BilinearClampedSampler, Input.UV0).rgb;

	float3 V = normalize(g_CameraWorldPosition.xyz - WorldPosition);
	float3 R = reflect(-V, N);


	[branch]
	if(g_DirectionalLightDir.w > 0.0f)
	{
		float3 L = normalize(g_DirectionalLightDir.xyz);
		float3 H = normalize(V + L);

		float attenuation = 1.0f; //DirectionLight has no transport attenuation
		float3 radiance = g_DirectionalLightColor.rgb * attenuation;// Get the L(p, wi)

		// Cook-Torrance BRDF
		float NDF = D_GGX(N, H, roughness);
		float G = Vis_GeometrySmith(N, V, L, roughness);
		float3 F = Fresnel_Schlick(max(dot(H, V), 0.0), F0);

		float3 KS = F;
		float3 KD = float3(1.0f, 1.0f, 1.0f) - KS;
		KD *= 1.0f - metallic;
		
		float3 nominator = NDF * G * F;
		float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f);
		float3 specular = nominator / max(denominator, 0.001f);

		// combine
		float NDotL = max(dot(N, L), 0.0f);
		Lo += (KD * albedo / PI + specular) * radiance * NDotL;
	}

	float3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	float3 kS = F;
	float3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;

	// -------------IBL Lighting-----------------------//
	float3 irradiance = diffuseIrradianceMap.Sample(BilinearWrapSampler, N).rgb;
	//return float4(irradiance, 1.0f);

	float3 indirectDiffuse = irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0f;
	float3 prefilteredColor = specularPrefilterMap.SampleLevel(TrilinearWrapSampler, R, roughness * MAX_REFLECTION_LOD).rgb;
	float2 envBRDF = specularBRDFLutMap.Sample(TrilinearWrapSampler, float2(max(dot(N, V), 0.0), roughness)).rg;
	float3 indirectSpecular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
	// ------------------------------------------------//
	float3 ambient = (kD * indirectDiffuse + indirectSpecular) * ao;
	
	//float3 ambient = float3(0.03, 0.03, 0.03) * Albedo * ao;
	float3 color = ambient + Lo;
	
	// Tonemap
	color = color / (color + float3(1.0f, 1.0f, 1.0f));
	
	// Gamma Inverse Correct
	float invGamma = 1.0f / 2.2f;
	color = pow(color, float3(invGamma, invGamma, invGamma));

	return float4(color, 1.0f);
}