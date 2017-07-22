//Lighting pass (and geometry pass) includes
#include "../Includes/HLSLLightPassIncludes.hlsl"

//cbuffer register(b0) == camera data

cbuffer CB_LightPass_DirectionalLightData : register(b1)
{
    float4 LightDirection;
    float4 LightColourAndIntensity;
};

//Output from the VS to the PS
struct VSOutput
{
    float4 V_Position : SV_Position;
    float2 C_Position2D : POSITION;
};

float4 main(VSOutput input) : SV_Target
{
    //Sample GBuffer at this pixel + unpack. Note: input.H_Position has
    //undergone the viewport transform so represents the pixel we wish to
    //sample the GBuffer for (via Load())
    UnpackedGBufferSampleData gBufferSample = UnpackGBuffer(int2(input.V_Position.xy));

    //World space position
    float3 pixelWorldSpace = ReconstructWorldSpacePosition(input.C_Position2D, gBufferSample.LinearDepth);

    //Directional lighting
    float3 c = LightPass_DirectionalLight(float3(LightDirection.xyz),
        float3(LightColourAndIntensity.xyz), LightColourAndIntensity.w,
        gBufferSample.SpecularIntensity, gBufferSample.SpecularPower,
        gBufferSample.Colour, gBufferSample.Normal, pixelWorldSpace);
    
    //TODO: Ambient - this is a temp hack :) 
    c.rgb += gBufferSample.Colour * float3(0.1f, 0.1f, 0.1f);
    //c.rgb +=  float3(0.1f, 0.1f, 0.1f);


    //Done
    return float4(c.rgb, 0.f);
} 