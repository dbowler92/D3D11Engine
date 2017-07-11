//Input to VS
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Colour : COLOUR;
};

struct VS_OUTPUT
{
    float4 Positon : SV_Position;
    float3 Colour : COLOUR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT o;
    o.Positon = float4(input.Position.xyz, 1.0f);
    o.Colour = input.Colour;

    return o;
}