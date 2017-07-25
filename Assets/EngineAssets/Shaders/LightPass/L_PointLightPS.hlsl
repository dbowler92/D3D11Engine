//Lighting pass (and geometry pass) includes
#include "../Includes/HLSLLightPassIncludes.hlsl"

//cbuffer register(b0) == camera data

cbuffer CB_LightPass_PointLightData : register(b1)
{
    float3 LightPosition;
    float  LightRange;
    float4 LightColourAndIntensity;
    float4x4 LightWorldViewProjection;
};

//Output from the DS to the PS
struct DSOutput
{
    float4 V_Position   : SV_Position;  //Undergone viewport transform - sample GBuffer
    float2 C_Position2D : TEXCOORD0;
};


float4 main(DSOutput input) : SV_Target
{
    //TEMP
    return float4(0.0f, 0.0f, 1.f, 1.f);
}