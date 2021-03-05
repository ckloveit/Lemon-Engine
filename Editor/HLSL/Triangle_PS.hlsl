#include "Triangle.hlsli"

// 像素着色器
float4 PS(VertexOut pIn) : SV_Target0
{
    return pIn.color;
}

