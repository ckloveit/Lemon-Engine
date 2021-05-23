//Low frequence- Update only per frame
cbuffer ViewUniformBuffer: register(b0)
{
    matrix g_ViewMatrix;
    matrix g_ProjectionMatrix;
    matrix g_ViewProjectionMatrix;
    float4 g_CameraWorldPosition;
    float3 g_TestColor; 
};

// High frequency - Updates at least as many times as there are objects in the scene
cbuffer ObjectUniformBuffer : register(b1)
{
    matrix g_LocalToWorldMatrix;
    matrix g_WorldToWorldMatrix;
    matrix g_WorldToWorldTransposeMatrix;

    //PBR Properties
    float4 g_Albedo;
    float4 g_PBRParameters; //x : Metallic; y : Roughness; z : AO 
    
    matrix g_LocalColor;
};

// Low frequency buffer - Updates once per frame
cbuffer LightUniformBuffer : register(b2)
{
    float4 g_DirectionalLightDir;
    float4 g_DirectionalLightColor;
};

cbuffer FullScreenUniformBuffer : register(b3)
{
	matrix g_FullScreen_LocalToWorldMatrix;
};