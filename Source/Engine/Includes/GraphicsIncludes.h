//GraphicsIncludes.h
//Created 26/04/17
//Created by Daniel Bowler
//
//Simple header that will include all graphics headers required

#pragma once

//Globals
#include "../../Main/EngineStd.h"

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include <Windows.h>
#include <d3dx11.h>
#include "../3rdParty/d3dx11Effect.h"
#include <xnamath.h>
#include <dxerr.h>
#endif