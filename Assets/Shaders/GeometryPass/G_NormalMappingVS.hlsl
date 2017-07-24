#include "../../EngineAssets/Shaders/Includes/HLSLGeometryPassIncludes.hlsl"

cbuffer CB_VS_PerObject : register(b1)
{
    float4x4 World;
};

//Input to VS
struct VS_INPUT
{
    float3 L_Position : POSITION;
    float3 L_Normal   : NORMAL;
    float3 L_Tangent  : TANGENT;
    float2 Texcoord   : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 H_Positon   : SV_Position;
    float3 W_Normal    : NORMAL; 
    float3 W_Tangent   : TANGENT;
    float3 W_Bitangent : BITANGENT;
    float2 Texcoord    : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{   
    //float4 ws = mul(float4(input.L_Position.xyz, 1.0f), World);
    //float4 hcs = mul(ws, CameraViewProj);
    float4 clipspace = LocalPositionToClipSpace(input.L_Position, World, CameraViewProj);
     
    VS_OUTPUT o;
    o.H_Positon = clipspace;
    o.W_Normal = normalize(mul(input.L_Normal.xyz, (float3x3)World));
    o.Texcoord = input.Texcoord;

    //Binormal/tangent vector
    o.W_Tangent = normalize(mul(input.L_Tangent.xyz, (float3x3) World));
    o.W_Bitangent = normalize(cross(o.W_Normal, o.W_Tangent));

    //Done
    return o;
}