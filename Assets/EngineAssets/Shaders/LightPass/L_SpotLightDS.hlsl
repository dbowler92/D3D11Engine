//Lighting pass (and geometry pass) includes
#include "../Includes/HLSLLightPassIncludes.hlsl"

//Consts
static const float PI = 3.14159265f;

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

    //Range and direction
    float3 lsDirection = float3(0.f, 0.f, +1.f);
    float lsHeight = 1.f;

    //Calculate the radius of our cone in LS
    float lsRadius = lsHeight * tan(radians(halfAngle));

    //Transform quad verts From [0,1] to [-1, 1]
    float2 clipSpacePos = uv.xy * 2.0f - 1.0f;

    //Flip uv.y so that 0 is at the top, 1 at the bottom
    //uv.y = (1.f - uv.y);

    //Wrap quad around the cone - generates a local space cone
    float x = lsHeight - uv.y / lsHeight * lsRadius * cos((uv.x * (2.0f * PI)));
    float y = lsHeight - uv.y / lsHeight * lsRadius * sin((uv.x * (2.0f * PI)));
    float z = uv.y * lsHeight;

    //To clip space from local space
    float4 posL = float4(x, y, z, 1.f);
    float4 posC = mul(posL, LightWorldViewProjection);
     
    //Output
    DSOutput o;
    o.C_Position = posC;
    //o.C_Position = float4(clipSpacePos.xy, 1.f, 1.f);
    o.C_Position2D = o.C_Position.xy / o.C_Position.w;
    o.Data = float4(LightColourAndIntensity.rgb, 1.f);
    return o;
}