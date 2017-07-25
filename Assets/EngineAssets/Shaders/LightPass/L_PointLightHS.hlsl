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
    o.Inside[1] = TESS_FACTOR;

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
    //Simple function which is called per patch. We will
    //draw a point light wtih 2 control points. This function 
    //will return a single patch for both sides of the sphere (thus, we
    //have 2 half spheres). 
    HSOutput o;
    o.HemiPosition = HemiDir[patchID];
    return o;
}