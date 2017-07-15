//Constant buffer data
cbuffer WVPCBuffer : register(b0)
{
    float4x4 WVP;
};

//Input to VS
struct VS_INPUT
{
    float3 Position : POSITION;
    float2 Texcoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Positon : SV_Position;
    float2 Texcoord : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT o;
    o.Positon = mul(float4(input.Position.xyz, 1.0f), WVP);
    o.Texcoord = input.Texcoord;

    return o;
}