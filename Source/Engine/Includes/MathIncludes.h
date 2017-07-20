//MathIncludes.h
//Created 11/07/17
//Created by Daniel Bowler
//
//Simple header that will include all required math files

#pragma once

//
//DirectXMath : Matricies are stored row-major, row vector math.
//HLSL        : Expects matricies in column-major order. Therefore, we have to 
//				transpose matricies before sending to a shader. (row and colum
//				ectors: mul(M, v) and mul(v, M)) both work fine)
//
//#include <xnamath.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include <DirectXColors.h>

using namespace DirectX; 

//
//Use GLM as another option??
//
//GLM
#define GLM_FORCE_RADIANS
#include <3rdParty/glm/glm/glm.hpp>
#include <3rdParty/glm/glm/gtc/matrix_transform.hpp>
#include <3rdParty/glm/glm/gtc/quaternion.hpp>

using namespace glm;

//
//My own simple math library - TODO
//
#include <Math/Math.h>

using namespace EngineAPI::Math;