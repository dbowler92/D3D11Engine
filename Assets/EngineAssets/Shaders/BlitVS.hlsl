//Blits a SRV in to an RTV - Eg: Debug rendering
//
//We generate the quad in the VS
static const float2 Verts[4] =
{
    float2(-1.0f, 1.0f),
    float2(1.0f, 1.0f),
    float2(-1.0f, -1.0f),
    float2(1.0f, -1.0f),
};

//Output from the VS to the PS
struct VSOutput
{
    float4 C_Position : SV_Position;
};

VSOutput main(uint vertexID : SV_VertexID)
{
    VSOutput o;
    o.C_Position = float4(Verts[vertexID].xy, 0.0f, 1.0f);
    return o;
}