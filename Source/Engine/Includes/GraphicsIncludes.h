//GraphicsIncludes.h
//Created 26/04/17
//Created by Daniel Bowler
//
//Simple header that will include all graphics headers required

#pragma once

//Engine Globals
#include "../../Main/EngineStd.h"

//Graphics globals
#include "../Globals/GraphicsGlobals.h"

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include <dxgi.h>
#include <d3d11.h>
#include "../3rdParty/dxerr/dxerr.h"
#endif