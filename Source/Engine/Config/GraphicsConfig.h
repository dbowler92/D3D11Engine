//GraphicsConfig.h
//Created 04/05/17
//Created By Daniel Bowler
//
//Some shared graphics settings - for all APIs whenever they get implemented!

#pragma once

//1 == no MSAA, > 1 == MSAA (Max 16)
#define GRAPHICS_CONFIG_MSAA_SAMPLE_COUNT 4
//Backbuffer count. Note: 1 == Double buffering && 2 == tripple buffering
#define GRAPHICS_CONFIG_BACKBUFFER_COUNT 1
//Should we create a depth stencil buffer alongside the backbuffers? true | false
#define GRAPHICS_CONFIG_DO_CREATE_DEPTH_BUFFER_WITH_SWAPCHAIN true

//Winding order of the front face => 1 == clockwise, 0 == counter clockwise front faces
#define GRAPHICS_CONFIG_DEFAULT_FRONT_FACE_WINDING_ORDER_IS_CLOCKWISE 1