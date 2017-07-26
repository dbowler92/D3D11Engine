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
    //From [0,1] to [-1, 1]
    float2 clipSpacePos = uv.xy * 2.0f - 1.0f;

    //Absolute distance to the center (0, 0)
    float2 distToMiddle = abs(clipSpacePos.xy);
    float maxLen = max(distToMiddle.x, distToMiddle.y);

    //Position on half sphere in clipspace
    float3 normDir = normalize(float3(clipSpacePos.xy, maxLen - 1.0f) * quad[0].HemiPosition);
    float4 posL = float4(normDir.xyz, 1.f);

    //Local to clip space
    float4 posC = mul(posL, LightWorldViewProjection);

    //Output
    DSOutput o;
    o.C_Position = posC;
    o.C_Position2D = o.C_Position.xy / o.C_Position.w;
    return o;
}