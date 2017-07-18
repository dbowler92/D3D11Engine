//Input to PS
struct VS_OUTPUT
{
    float4 Positon  : SV_Position;
    float2 Texcoord : TEXCOORD0;
};

//Output
struct PSOutput
{
    float4 Target0 : SV_TARGET0;
    float4 Target1 : SV_TARGET1;
    float4 Target2 : SV_TARGET2;
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
    o.Target1 = float4(1.0f, 0.0f, 0.0f, 1.0f);
    o.Target2 = float4(0.0f, 1.0f, 0.0f, 1.0f);

    return o;
}