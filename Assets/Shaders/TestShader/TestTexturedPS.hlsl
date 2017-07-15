//Input to PS
struct VS_OUTPUT
{
    float4 Positon  : SV_Position;
    float2 Texcoord : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
    return float4(input.Texcoord.rg, 0.0f, 1.0f);
}