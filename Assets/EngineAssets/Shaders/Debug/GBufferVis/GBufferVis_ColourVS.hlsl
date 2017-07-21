//We generate the quad in the VS
static const float2 Verts[4] =
{
    float2(-1.0f, 1.0f),
    float2(1.0f, 1.0f),
    float2(-1.0f, -1.0f),
    float2(1.0f, -1.0f),
};

static const float2 Tex[4] =
{
    float2(0.f, 0.f),
    float2(1.f, 0.f),
    float2(0.f, 1.f),
    float2(1.f, 1.f),
};

//Output from the VS to the PS
struct VSOutput
{
    float4 C_Position : SV_Position;
    float2 Texcoord   : TEXCOORD;
};

VSOutput main(uint vertexID : SV_VertexID)
{
    VSOutput o;
    o.C_Position = float4(Verts[vertexID].xy, 0.0f, 1.0f);
    o.Texcoord = Tex[vertexID].xy;
    return o;
}