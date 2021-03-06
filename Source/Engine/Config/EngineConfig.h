//EngineConfig.h
//Created 15/04/17
//Created By Daniel Bowler
//
//Used to set the state of the game engine

#pragma once

//Engine name
#define ENGINE_NAME "Real Engine 4 D3D11"

//Engine version
#define ENGINE_VERSION_MAJOR 0
#define ENGINE_VERSION_MINOR 3
#define ENGINE_VERSION_PATCH 0

//Folder for debug dumps
#define DEBUG_DUMPS_FOLDER "Dumps/"

//Application assets folders 
#define ASSETS_FOLDER "../Assets/"
#define SHADER_ASSETS_FOLDER ASSETS_FOLDER"Shaders/"
#define SHADER_COMPILED_ASSETS_FOLDER SHADER_ASSETS_FOLDER"Compiled/"
#define TEXTURE_ASSETS_FOLDER ASSETS_FOLDER"Textures/"

//Engine assets folder
#define ENGINE_ASSETS_FILDER ASSETS_FOLDER"/EngineAssets/"
#define ENGINE_SHADER_ASSETS_FOLDER ENGINE_ASSETS_FILDER"Shaders/"
#define ENGINE_SHADER_COMPILED_ASSETS_FOLDER ENGINE_SHADER_ASSETS_FOLDER"/Compiled/"
#define ENGINE_TEXTURE_ASSETS_FOLDER ENGINE_ASSETS_FILDER"/Textures/"

//Platform - not really used for now?
#define PLATFORM_IDX_WIN32 0

//Graphics APIs 
#define GRAPHICS_API_IDX_D3D11 0

//Current platform to build & grpahics API to use - this should really be set
//through a VS build configuration. But for now, this will do.  
#define ENGINE_BUILD_SETTINGS_PLATFORM PLATFORM_IDX_WIN32
#define ENGINE_BUILD_SETTINGS_GRAPHICS_API GRAPHICS_API_IDX_D3D11

//Should stop executing program on warning or error messages?
#define ENGINE_CONFIG_STOP_EXECUTION_ON_ERROR_MSG 1
#define ENGINE_CONFIG_STOP_EXECUTION_ON_WARNING_MSG 0

//
//PLATFORM
//
//So we can use #ifdef and #ifndef
#if ENGINE_BUILD_SETTINGS_PLATFORM == PLATFORM_IDX_WIN32
#define ENGINE_CONFIG_PLATFORM_WIN32

//Win32 defines and the like
#include "Win32Config.h"

#endif

//So we can use #ifdef and #ifndef
#if ENGINE_BUILD_SETTINGS_GRAPHICS_API == GRAPHICS_API_IDX_D3D11
#define ENGINE_CONFIG_GRAPHICS_API_D3D11

//D3D11 config
#include "D3D11Config.h"

#endif
