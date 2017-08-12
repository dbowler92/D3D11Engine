//Constant hull shader - defines the tessellation factor
struct ConstantHSOut
{
    float Edges[4] : SV_TessFactor;
    float Inside[2] : SV_InsideTessFactor;
};

ConstantHSOut CHSMain()
{
    const float TESS_FACTOR = 18.0f;

    ConstantHSOut o;
    o.Edges[0] = TESS_FACTOR;
    o.Edges[1] = TESS_FACTOR;
    o.Edges[2] = TESS_FACTOR;
    o.Edges[3] = TESS_FACTOR;

    o.Inside[0] = TESS_FACTOR;
    o.Inside[1] = TESS_FACTOR; /// 2.0f;

    return o;
}

static const float3 HemiDir[2] =
{
    float3(1.0f, 1.0f, 1.0f),
    float3(-1.0f, 1.0f, -1.0f)
};

//Hull shader
struct HSOutput
{
    float3 HemiPosition : POSITION;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CHSMain")]
HSOutput main(uint patchID : SV_PrimitiveID)
{
    HSOutput o;
    o.HemiPosition = HemiDir[patchID];
    return o;
}