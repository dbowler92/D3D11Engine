//GBuffer SRV
Texture2D GBufferTexture : register(t0);

//Sampler state
sampler GBufferSamplerState : register(s0);

//Output from the VS to the PS
struct VSOutput
{
    float4 V_Position : SV_Position;
    float2 Texcoord   : TEXCOORD;
};

float4 main(VSOutput input) : SV_Target
{ 
    //[0,1] range
    float3 sample = GBufferTexture.Sample(GBufferSamplerState, input.Texcoord).xyz;

    //to [-1,1] range
    sample = sample * 2.0f - 1.0f;
    return float4(sample, 0.f);
}