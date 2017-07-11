//GraphicsGlobals.h
//Created 11/07/17
//Created By Daniel Bowler
//
//Global graphics stuff goes here!
//
//Note: This all looks similar(ish) to D3D11 - In the future, *if* I decide
//to add multi-API support (GL, GLES perhaps), I will want to absract the API
//in such a way that it mimics D3D11 in as many ways as is possible

#pragma once

//Essentially, the same as D3D11_USAGE
enum ResourceUsage
{
	RESOURCE_USAGE_DEFAULT = 0,			//GPU read-write
	RESOURCE_USAGE_IMMUTABLE = 1,	    //GPU read-only -> Data set at init time
	RESOURCE_USAGE_DYNAMIC = 2,	        //CPU write -> Via Map()
	RESOUCE_USAGE_STAGING = 3,			//CPU read.
};

//CPU usage incase of dynamic && staging buffers
enum ResourceCPUAccessBit
{
	RESOURCE_CPU_ACCESS_WRITE_BIT = 0x10000L,
	RESOURCE_CPU_ACCESS_READ_BIT = 0x20000L,
};

//Flag which indicates where we will be binding resources
enum ResourceBindBit
{
	RESOURCE_BIND_VERTEX_BUFFER_BIT = 0x1L,
	RESOURCE_BIND_INDEX_BUFFER_BIT = 0x2L,
	RESOURCE_BIND_CONSTANT_BUFFER_BIT = 0x4L,
	RESOURCE_BIND_SHADER_RESOURCE_BIT = 0x8L,
	RESOURCE_BIND_STREAM_OUTPUT_BIT = 0x10L,
	RESOURCE_BIND_RENDER_TARGET_BIT = 0x20L,
	RESOURCE_BIND_DEPTH_STENCIL_TARGET_BIT = 0x40L,
	RESOURCE_BIND_UNORDERED_ACCESS_BIT = 0x80L
};

typedef UINT ResourceBindFlag;
typedef UINT ResourceCPUAccessFlag;