//HLSLLightPassIncludes.h
//Created 20/07/17
//Created By Daniel Bowler
//
//Globals for light pass HLSL shaders

//Also includes the geometry pass data for
//reference - Includes the camera matrices which
//we will use to generate linear depth
#include "HLSLGeometryPassIncludes.hlsl"

//Defines
//
//Do we use blinn (half vector) or phong (reflect vector)?
#define DIRECTIONAL_LIGHT_USE_BLINN 0
#define POINT_LIGHT_USE_BLINN 0
#define SPOT_LIGHT_USE_BLINN 1

//Bind the GBuffer as shader resources during the
//lighting pass
Texture2D GBuffer_Depth                 : register(t0);
Texture2D GBuffer_DiffuseSpecIntensity  : register(t1);
Texture2D GBuffer_PackedNormal          : register(t2);
Texture2D GBuffer_SpecPower             : register(t3);

//Unpacked data from the GBuffer for a given pixel
struct UnpackedGBufferSampleData
{
    float  LinearDepth;
    float3 Colour;
    float3 Normal;
    float  SpecularIntensity;
    float  SpecularPower;
};

//Samples and unpacks the GBuffer
UnpackedGBufferSampleData UnpackGBuffer(int2 postViewportTransformPixelLocation)
{
    int3 sampleLoc = int3(postViewportTransformPixelLocation, 0);

    //Output
    UnpackedGBufferSampleData o;

    //Sample the GBuffer & extract the simple data (Will do depth
    //later)
    float4 sampled_Colour_SpecIntensity = GBuffer_DiffuseSpecIntensity.Load(sampleLoc).rgba;
    o.Colour = sampled_Colour_SpecIntensity.rgb;
    o.SpecularIntensity = sampled_Colour_SpecIntensity.a;
    
    o.Normal = GBuffer_PackedNormal.Load(sampleLoc).xyz;
    o.Normal = normalize((o.Normal * 2.0f - 1.0f)); //Convert from [0,1] to [-1,1]
   
    o.SpecularPower = GBuffer_SpecPower.Load(sampleLoc).r;
    o.SpecularPower = (SPEC_POWER_RANGE_MIN + SPEC_POWER_RANGE_MAX) * o.SpecularPower;

    //Linear depth
    float depthSample = GBuffer_Depth.Load(sampleLoc).r;
    float2 perspectiveValues = float2(CameraProj[3][2], -CameraProj[2][2]);
    o.LinearDepth = perspectiveValues.x / (depthSample + perspectiveValues.y);

    //Done
    return o;
}

//Takes a linear depth - as calculated above - and returns the world space
//position of the pixel
float3 ReconstructWorldSpacePosition(float2 clipSpacePosition, float linearDepth)
{
    //Invert projection effects on the clip space position
    //of the pixel - this gives us view space 
    float4 p;
    float2 perspectiveValues = float2(1.0f / CameraProj[0][0],
        1.0f / CameraProj[1][1]);

    p.xy = clipSpacePosition.xy * perspectiveValues.xy * linearDepth;
    p.z = linearDepth;
    p.w = 1.0f;

    //View to world
    return mul(p, CameraInverseView).xyz;
}

float3 LightPass_DirectionalLight(float3 lightDir, 
    float3 lightColour, float lightIntensity, 
    float specIntensity, float specPower, float3 surfaceDiffuse,
    float3 surfaceNormal_W, float3 surfacePositionW)
{
    float3 finalColourOut = float3(0.f, 0.f, 0.f);

    //Surface (A) to eye (B) (WS) == b-a
    float3 surfaceToEye = normalize(CameraWorldPosition - surfacePositionW);

    //Diffuse
    lightDir = normalize(lightDir);
    float3 negLightDir = -lightDir;
    float N_Dot_L = dot(negLightDir, surfaceNormal_W);
    finalColourOut = (lightColour.rgb * lightIntensity) * saturate(N_Dot_L);

#if DIRECTIONAL_LIGHT_USE_BLINN == 1
    //Spec - blinn phong
    float3 h = normalize(surfaceToEye + negLightDir);
    float N_Dot_H = saturate(dot(h, surfaceNormal_W));
    finalColourOut += (lightColour.rgb * lightIntensity) * pow(N_Dot_H, specPower) * specIntensity;
#else  
    //Spec - phong
    float3 r = reflect(lightDir, surfaceNormal_W);
    float angle = saturate(dot(r, surfaceToEye));
    finalColourOut += (lightColour.rgb * lightIntensity) * pow(angle, specPower) * specIntensity;
#endif

    //Return
    finalColourOut *= surfaceDiffuse;
    return finalColourOut;
}


float3 LightPass_PointLight(float3 lightPos, float lightRange,
    float3 lightColour, float lightIntensity,
    float specIntensity, float specPower, float3 surfaceDiffuse,
    float3 surfaceNormal_W, float3 surfacePositionW)
{
    float3 finalColourOut = float3(0.f, 0.f, 0.f);
 
    //Calculate a vector from the surface (A) to the 
    //middle of the light (B) => B-A
    //
    //Also calculate the length of this vector -> Used
    //to attenuate later
    float3 surfToLight = lightPos - surfacePositionW;
    float surfToLightDistance = length(surfToLight);
    surfToLight /= surfToLightDistance;

    //Vector from the surface (A) to the camera position (B)
    //B-A
    float3 surfToEye = normalize(CameraWorldPosition - surfacePositionW);

    //Diffuse
    float N_Dot_L = saturate(dot(surfToLight, surfaceNormal_W));
    finalColourOut = (lightColour.rgb * lightIntensity) * N_Dot_L;

#if POINT_LIGHT_USE_BLINN == 1
    //Spec - Blinn
    float3 h = normalize(surfToEye + surfToLight);
    float N_Dot_H = saturate(dot(h, surfaceNormal_W));
    finalColourOut += (lightColour.rgb * lightIntensity) * pow(N_Dot_H, specPower) * specIntensity;
#else 
    //Spec - Phong
    float3 r = reflect(-surfToLight, surfaceNormal_W);
    float angle = saturate(dot(r, surfToEye));
    finalColourOut += (lightColour.rgb * lightIntensity) * pow(angle, specPower) * specIntensity;
#endif

    //Attenuation - TODO: Better attenuation function + move the 1.0f/lightRange back to the
    //CPU
    float distToLightNormalized = 1.0f - saturate(surfToLightDistance * (1.0f / lightRange));
    float attenuation = distToLightNormalized * distToLightNormalized;

    //Done
    finalColourOut *= surfaceDiffuse * attenuation;
    return finalColourOut;
}