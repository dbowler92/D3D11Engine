//GraphicsConfig.h
//Created 04/05/17
//Created By Daniel Bowler
//
//Some shared graphics settings - for all APIs whenever they get implemented!

#pragma once

//Backbuffer count. Note: 1 == Double buffering && 2 == tripple buffering
#define GRAPHICS_CONFIG_BACKBUFFER_COUNT 1

//Winding order of the front face => 1 == clockwise, 0 == counter clockwise front faces
#define GRAPHICS_CONFIG_DEFAULT_FRONT_FACE_WINDING_ORDER_IS_CLOCKWISE 1