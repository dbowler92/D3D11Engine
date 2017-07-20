//HLSLLightPassIncludes.h
//Created 20/07/17
//Created By Daniel Bowler
//
//Globals for light pass HLSL shaders

//Also includes the geometry pass data for
//reference
#include "HLSLGeometryPassIncludes.hlsl"

//Point sampler for the GBuffer
sampler GBuffer_PointSampler : register(s0);

//Bind the GBuffer as shader resources during the
//lighting pass
Texture2D GBuffer_Depth                 : register(t0);
Texture2D GBuffer_DiffuseSpecIntensity  : register(t1);
Texture2D GBuffer_PackedNormal          : register(t2);
Texture2D GBuffer_SpecPower             : register(t3);

//Samples and unpacks the GBuffer