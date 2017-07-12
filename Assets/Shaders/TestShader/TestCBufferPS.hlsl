//Input to PS
struct VS_OUTPUT
{
    float4 Positon : SV_Position;
    float3 Colour : COLOUR;
};

float4 main(VS_OUTPUT input) : SV_Target
{
    return float4(input.Colour.rgb, 1.0f);
}