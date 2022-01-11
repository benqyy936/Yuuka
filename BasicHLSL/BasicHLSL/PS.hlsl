#include "Type.hlsli"

Texture2D g_Tex : register(t0);
SamplerState g_SamLinear : register(s0);

float4 PS(VS_OUTPUT psInput) : SV_TARGET
{
    return g_Tex.Sample(g_SamLinear,psInput.texcoord);
}