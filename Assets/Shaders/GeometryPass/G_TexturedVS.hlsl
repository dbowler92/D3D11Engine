#include "HLSLGeometryPassIncludes.hlsl"

//Constant buffer data
cbuffer CB_Camera : register(b0)
{
    float4x4 View;
    float4x4 Proj;
    float4x4 ViewProj;
};

cbuffer CB_VS_PerObject : register(b1)
{
    float4x4 World;
};

//Input to VS
struct VS_INPUT
{
    float3 L_Position : POSITION;
    float3 L_Normal   : NORMAL;
    float2 Texcoord   : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 H_Positon : SV_Position;
    float3 W_Normal  : NORMAL; 
    float2 Texcoord  : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{   
    float4 ws = mul(float4(input.L_Position.xyz, 1.0f), World);
    float4 hcs = mul(ws, ViewProj);

    VS_OUTPUT o;
    o.H_Positon = hcs;
    o.W_Normal = mul(input.L_Normal.xyz, (float3x3)World);
    o.Texcoord = input.Texcoord;

    return o;
}