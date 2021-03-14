//Low frequence- Update only per frame
cbuffer BufferFrame : register(b0)
{
    matrix g_View;
    float3 g_TestColor; 
};