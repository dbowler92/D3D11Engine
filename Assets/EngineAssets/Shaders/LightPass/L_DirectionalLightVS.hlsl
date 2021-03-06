//Lighting pass (and geometry pass) includes
#include "../Includes/HLSLLightPassIncludes.hlsl"

//Global data - in this shader, we will 
//call with a null bound vertex buffer (4 draw count) - we will
//generate the quad in the VS. Sample this array with
//the vertexID to obtain the position and texcoords
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
    float2 C_Position2D : POSITION;
};

VSOutput main(uint vertexID : SV_VertexID)
{
    VSOutput o;
    o.C_Position = float4(Verts[vertexID].xy, 0.0f, 1.0f);
    o.C_Position2D = o.C_Position.xy; //XY clip space position
    return o;
}