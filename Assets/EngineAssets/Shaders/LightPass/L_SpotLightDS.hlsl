//Lighting pass (and geometry pass) includes
#include "../Includes/HLSLLightPassIncludes.hlsl"

//Consts
static const float PI = 3.14159265f;

//Defines
//
//How much to scale the spot light cone by in
//local space - the volume doesn't 100% encapsulate
//the actual light source, hence the mini hack!
#define SPOT_LIGHT_SCALE_FACTOR 1.2f

//Base verts are offset along the +z axis.
#define BASE_Z_OFFSET 0.1f;

cbuffer CB_LightPass_SpotLightData : register(b1)
{
    float3   LightPosition;
    float    LightRange;

    float3   LightDirection;
    float    LightOuterAngle; //note: Degress

    float    LightInnerAngle; //note: Degrees
    float3   __PadF3;

    float4   LightColourAndIntensity;

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
    //
    //Cone
    //
    float halfAngle = LightOuterAngle;

    //Height of the cone is the range
    float lsHeight = LightRange;
    
    //Calculate the radius of our cone in LS
    float lsRadius = lsHeight * tan(radians(halfAngle));

    //Transform quad verts From [0,1] to [-1, 1]
    float2 clipSpacePos = uv.xy * 2.0f - 1.0f;

    //Wrap quad around the cone - generates a local space cone
    float yRatio = uv.y * lsHeight;
    float xAngleRadians = uv.x * (2.0f * PI);

    float x = ((lsHeight - yRatio) / lsHeight) * lsRadius * cos(xAngleRadians);
    float y = ((lsHeight - yRatio) / lsHeight) * lsRadius * sin(xAngleRadians);
    float z = yRatio - lsHeight;
    z *= -1.f; //Flip ls direction -> LS cone should be pointing in the +Z axis

    //Base of cone.
    float baseFactor = (float) (int) (uv.y + 0.99f); //1 if uv.y != 0.0f. 0  if uv.y == 0.0f;

    float xBase = 0.0f; 
    float yBase = 0.0f;
    float zBase = z + BASE_Z_OFFSET;

    x = lerp(xBase, x, baseFactor);
    y = lerp(yBase, y, baseFactor);
    z = lerp(zBase, z, baseFactor);

    //Scale the cone
    float3 local = float3(x, y, z);
    local *= SPOT_LIGHT_SCALE_FACTOR;

    //To clip space from local space
    float4 posL = float4(local, 1.f);
    float4 posC = mul(posL, LightWorldViewProjection);
     
    //Output
    DSOutput o;
    o.C_Position = posC;
    //o.C_Position = float4(clipSpacePos.xy, 1.f, 1.f);
    o.C_Position2D = o.C_Position.xy / o.C_Position.w;
    return o;
}