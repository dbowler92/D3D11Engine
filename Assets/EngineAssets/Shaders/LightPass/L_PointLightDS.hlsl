//Lighting pass (and geometry pass) includes
#include "../Includes/HLSLLightPassIncludes.hlsl"

cbuffer CB_LightPass_PointLightData : register(b1)
{
    float3 LightPosition;
    float LightRange;
    float4 LightColourAndIntensity;
    float4x4 LightWorldViewProjection;
};

//Inputs to DS
struct ConstantHSOut
{
    float Edges[4] : SV_TessFactor;
    float Inside[2] : SV_InsideTessFactor;
};

struct HSOutput
{
    float3 HemiPosition : POSITION;
};

//Output from the DS
struct DSOutput
{
    float4 C_Position : SV_Position; //Required + will undergo viewport transform
    float2 C_Position2D : TEXCOORD0; //Clip pos for world space reconstruction
};

[domain("quad")]
DSOutput main(ConstantHSOut cInput, float2 uv : SV_DomainLocation, 
    const OutputPatch<HSOutput, 4> quad)
{
    DSOutput o;
    o.C_Position = float4(quad[0].HemiPosition, 1.0f);
    o.C_Position2D = o.C_Position.xy;
    return o;
}