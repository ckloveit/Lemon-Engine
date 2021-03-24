//Low frequence- Update only per frame
cbuffer BufferFrame : register(b0)
{
    matrix g_ViewMatrix;
    matrix g_ProjectionMatrix;
    matrix g_ViewProjectionMatrix;
    float3 g_TestColor; 
};


// High frequency - Updates at least as many times as there are objects in the scene
cbuffer ObjectFrame : register(b1)
{
    matrix g_LocalToWorldMatrix;
    matrix g_LocalColor;
};


