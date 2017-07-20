//Lighting pass (and geometry pass) includes
#include "../Includes/HLSLLightPassIncludes.hlsl"

cbuffer CB_LightPass_DirectionalLightData : register(b0)
{
    float4 LightDirection;
    float4 LightColourAndIntensity;
}

//Output from the VS to the PS
struct VSOutput
{
    float4 H_Position : SV_Position;
    float2 GBufferTexcoord : TEXCOORD;
};

float4 main(VSOutput input) : SV_Target
{
    //Sample GBuffer at this pixel + unpack
    float4 diffSpecIntensitySample = GBuffer_DiffuseSpecIntensity.Sample(GBuffer_PointSampler, input.GBufferTexcoord).rgba;

    //Diffuse

    //Specular

    return float4(diffSpecIntensitySample.rgb, 0.f);
} 