//Input to PS
struct VS_OUTPUT
{
    float4 Positon  : SV_Position;
    float2 Texcoord : TEXCOORD0;
};

SamplerState DefaultSamplerState
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};

//Smapler
Texture2D Tex : register(t0);

float4 main(VS_OUTPUT input) : SV_Target
{
    //Sample
    float4 texSample = Tex.Sample(DefaultSamplerState, input.Texcoord);
    return float4(texSample.rgb, 1.0f);
}