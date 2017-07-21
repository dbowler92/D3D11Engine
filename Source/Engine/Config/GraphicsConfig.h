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

//Bindings
//
//Camera: Always bound in slot 0 for geometry pass, lighting pass && in the future
//forward passes (eg: translucent geometry)
#define GRAPHICS_CONFIG_CAMERA_CBUFFER_BINDING_SLOT 0 

//Light data: During the lighting pass, the CBuffer representing
//each light will be bound in slot 0.
#define GRAPHICS_CONFIG_LIGHT_PASS_LIGHT_DATA_CBUFFER_BINDING_SLOT 1