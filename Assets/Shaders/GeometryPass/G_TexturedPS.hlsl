//Input to PS
struct VS_OUTPUT
{
    float4 H_Positon : SV_Position;
    float3 W_Normal  : NORMAL;
    float2 Texcoord  : TEXCOORD0;
};

//Output
struct PSOutput
{
    float4 Target0 : SV_TARGET0;
    float4 Target1 : SV_TARGET1;
    float4 Target2 : SV_TARGET2;
};

cbuffer CB_PS_PerObject : register(b2)
{
    //Material data
    float SpecPower;
    float SpecIntensity;
    float _Pad0;
    float _Pad1;
};

//Sampler state
sampler LinearSamplerState : register(s0);

//Texture
Texture2D Tex : register(t0);

PSOutput main(VS_OUTPUT input) : SV_Target
{
    //Sample
    float4 texSample = Tex.Sample(LinearSamplerState, input.Texcoord);
    //return float4(texSample.rgb, 1.0f);

    PSOutput o;
    o.Target0 = float4(texSample.rgb, 1.0f);
    o.Target1 = float4(input.W_Normal, 1.0f);
    o.Target2 = float4(SpecIntensity, SpecIntensity, SpecIntensity, 1.0f);

    return o;
}