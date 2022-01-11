struct VS_INPUT
{
    float4 pos:POSITION;
    float2 texcoord:TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos:SV_POSITION;
    float2 texcoord:TEXCOORD;
};