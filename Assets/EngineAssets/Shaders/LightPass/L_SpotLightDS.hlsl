//Lighting pass (and geometry pass) includes
#include "../Includes/HLSLLightPassIncludes.hlsl"

//Defines
//
//How much to scale the point light sphere by in
//local space
#define POINT_LIGHT_SCALE_FACTOR 1.2f

cbuffer CB_LightPass_SpotLightData : register(b1)
{
    float3 LightPosition;
    float LightRange;
    float3 LightDirection;
    float LightAngle;
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
    float4 Data : COLOUR;
};

[domain("quad")]
DSOutput main(ConstantHSOut cInput, float2 uv : SV_DomainLocation, 
    const OutputPatch<HSOutput, 4> quad)
{
    //
    //Cone
    //
    float halfAngle = LightAngle * 0.5f;

    //Range of 1 in local space - scaled later
    float3 lsDirection = float3(0.f, -1.f, 0.f);
    float lsHeight = 1.f;

    //Calc slant vector and height to work out
    //base radius (pythagoras)
    //
    //Slant vector -> rotate the lsDirection by halfAngle
    float x = lsDirection.x * cos(halfAngle) - lsDirection.y * sin(halfAngle);
    float y = lsDirection.x * sin(halfAngle) + lsDirection.y * cos(halfAngle);
    float3 lsSlantVec = float3(x, y, 0.f);
    float lsSlantHeight = length(lsSlantVec);
    float baseRadius = sqrt((lsSlantHeight * lsSlantHeight) - (lsHeight * lsHeight));

    //Transform quad verts From [0,1] to [-1, 1]
    float2 clipSpacePos = uv.xy * 2.0f - 1.0f;

    //Clip space pos
    float2 cs = clipSpacePos;

    //Output
    DSOutput o;
    o.C_Position = float4(cs, 1.f, 1.f);
    o.C_Position2D = o.C_Position.xy / o.C_Position.w;
    o.Data = float4(uv.xy, 0.f, 1.f);
    return o;
}