#include "../../EngineAssets/Shaders/Includes/HLSLGeometryPassIncludes.hlsl"

//Input to PS
struct VS_OUTPUT
{
    float4 V_Positon   : SV_Position;
    float3 W_Normal    : NORMAL;
    float3 W_Tangent   : TANGENT;
    float3 W_Bitangent : BITANGENT;
    float2 Texcoord    : TEXCOORD0;
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
Texture2D DiffuseTexture : register(t0);
Texture2D NormalMapTexture : register(t1);

GeometryPassPackedGBufferPSOutput main(VS_OUTPUT input)
{
    //Sample tex for diffuse colour
    float4 diffuseTexSample = DiffuseTexture.Sample(LinearSamplerState, input.Texcoord);

    //Normal mapping
    float3 normalSample = NormalMapTexture.Sample(LinearSamplerState, input.Texcoord).xyz;
    normalSample = normalize((normalSample * 2.0f - 1.0f)); //[0,1] range to [-1,1]
    float3 worldNormal = ConvertNormalFromTangentToWorld(normalSample,
        input.W_Normal, input.W_Tangent, input.W_Bitangent);

    //Pack Gbuffer and return 
    return PackGBuffer(diffuseTexSample.rgb, worldNormal,
        SpecIntensity, SpecPower);
}