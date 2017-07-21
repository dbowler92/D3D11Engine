#include "../../EngineAssets/Shaders/Includes/HLSLGeometryPassIncludes.hlsl"

//Input to PS
struct VS_OUTPUT
{
    float4 H_Positon : SV_Position;
    float3 W_Normal  : NORMAL;
    float2 Texcoord  : TEXCOORD0;
};

cbuffer CB_PS_PerObject : register(b2)
{
    //Material data
    float SpecPower;
    float SpecIntensity;
    float CB_PS_PerObject_Pad0;
    float CB_PS_PerObject_Pad1;
};

//Sampler state
sampler LinearSamplerState : register(s0);

//Texture
Texture2D Tex : register(t0);

GeometryPassPackedGBufferPSOutput main(VS_OUTPUT input)
{
    //Sample tex for diffuse colour
    float4 texSample = Tex.Sample(LinearSamplerState, input.Texcoord);

    //Pack Gbuffer and return 
    return PackGBuffer(texSample.rgb, normalize(input.W_Normal),
        SpecIntensity, SpecPower);
}