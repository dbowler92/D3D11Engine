//Input to PS
struct VS_OUTPUT
{
    float4 Positon  : SV_Position;
    float2 Texcoord : TEXCOORD0;
};

//Sampler state
sampler LinearSamplerState : register(s0);

//Texture
Texture2D Tex : register(t0);

float4 main(VS_OUTPUT input) : SV_Target
{
    //Sample
    float4 texSample = Tex.Sample(LinearSamplerState, input.Texcoord);
    return float4(texSample.rgb, 1.0f);
}