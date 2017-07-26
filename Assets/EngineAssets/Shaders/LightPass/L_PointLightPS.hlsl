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
     //Sample GBuffer at this pixel + unpack. Note: input.H_Position has
    //undergone the viewport transform so represents the pixel we wish to
    //sample the GBuffer for (via Load())
    UnpackedGBufferSampleData gBufferSample = UnpackGBuffer(int2(input.V_Position.xy));

    //World space position
    float3 pixelWorldSpace = ReconstructWorldSpacePosition(input.C_Position2D, gBufferSample.LinearDepth);

    //Handle point lighting
    float3 c = LightPass_PointLight(LightPosition, LightRange,
        float3(LightColourAndIntensity.xyz), LightColourAndIntensity.w,
        gBufferSample.SpecularIntensity, gBufferSample.SpecularPower,
        gBufferSample.Colour, gBufferSample.Normal, pixelWorldSpace);

    //TEMP
    return float4(c.rgb, 1.f);
}