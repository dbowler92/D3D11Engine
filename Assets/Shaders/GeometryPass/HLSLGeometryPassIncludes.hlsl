//HLSLGeometryPassIncludes.h
//Created 20/07/17
//Created By Daniel Bowler
//
//Globals for GeometryPass HLSL shaders - All shaders
//will include this as it will contain the vital code
//to pack the GBuffer with data and the like. 

//
//Engine includes
//

float4 LocalPositionToWorld(float3 local, float4x4 world)
{
	return mul(float4(local, 1.0f), world);
}

