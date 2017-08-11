//Lighting pass (and geometry pass) includes
#include "../../Includes/HLSLLightPassIncludes.hlsl"

//cbuffer register(b0) == camera data

cbuffer CB_LightPass_SpotLightData : register(b1)
{
    float3 LightPosition;
    float  LightRange;

    float3 LightDirection;
    float  LightOuterAngle;

    float  LightInnerAngle;
    float3 __PadF3;

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
    //Returns the light colour
    return float4(LightColourAndIntensity.rgb, 1.0f);

    //Light colour scaled by intensity. 
    //float3 lightColScaledByIntensity = LightColourAndIntensity.rgb * LightColourAndIntensity.a;
    //return float4(lightColScaledByIntensity, 1.0f);
}