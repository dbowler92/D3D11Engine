//GraphicsGlobals.h
//Created 11/07/17
//Created By Daniel Bowler
//
//Global graphics stuff goes here!
//
//Note: This all looks similar(ish) to D3D11 - In the future, *if* I decide
//to add multi-API support (GL, GLES perhaps), I will want to absract that API
//in such a way that it mimics D3D11 in as many ways as is possible

#pragma once

//Rendering/primitive topology
enum PrimitiveTopology
{
	PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
	PRIMITIVE_TOPOLOGY_POINTLIST = 1,  
	PRIMITIVE_TOPOLOGY_LINELIST = 2,
	PRIMITIVE_TOPOLOGY_LINESTRIP = 3,
	PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
	PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,
	PRIMITIVE_TOPOLOGY_LINELIST_ADJ = 10,
	PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ = 11,
	PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ = 12,
	PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ = 13,
	PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST = 33,
	PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST = 34,
	PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST = 35,
	PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST = 36,
	PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST = 37,
	PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST = 38,
	PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST = 39,
	PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST = 40,
	PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST = 41,
	PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST = 42,
	PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST = 43,
	PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST = 44,
	PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST = 45,
	PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST = 46,
	PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST = 47,
	PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST = 48,
	PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST = 49,
	PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST = 50,
	PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST = 51,
	PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST = 52,
	PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST = 53,
	PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST = 54,
	PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST = 55,
	PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST = 56,
	PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST = 57,
	PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST = 58,
	PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST = 59,
	PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST = 60,
	PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST = 61,
	PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST = 62,
	PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST = 63,
	PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST = 64,
};

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

//Resource formats - maps directly to DXGI.
//
//TODO: Improve: A system to map from D3D11/DXGI to Gl types + remove
//support for those that don't exist in both platforms (which can not be
//hacked in some clever way)
enum ResourceFormat
{
	RESOURCE_FORMAT_UNKNOWN = 0,
	RESOURCE_FORMAT_R32G32B32A32_TYPELESS = 1,
	RESOURCE_FORMAT_R32G32B32A32_FLOAT = 2,
	RESOURCE_FORMAT_R32G32B32A32_UINT = 3,
	RESOURCE_FORMAT_R32G32B32A32_SINT = 4,
	RESOURCE_FORMAT_R32G32B32_TYPELESS = 5,
	RESOURCE_FORMAT_R32G32B32_FLOAT = 6,
	RESOURCE_FORMAT_R32G32B32_UINT = 7,
	RESOURCE_FORMAT_R32G32B32_SINT = 8,
	RESOURCE_FORMAT_R16G16B16A16_TYPELESS = 9,
	RESOURCE_FORMAT_R16G16B16A16_FLOAT = 10,
	RESOURCE_FORMAT_R16G16B16A16_UNORM = 11,
	RESOURCE_FORMAT_R16G16B16A16_UINT = 12,
	RESOURCE_FORMAT_R16G16B16A16_SNORM = 13,
	RESOURCE_FORMAT_R16G16B16A16_SINT = 14,
	RESOURCE_FORMAT_R32G32_TYPELESS = 15,
	RESOURCE_FORMAT_R32G32_FLOAT = 16,
	RESOURCE_FORMAT_R32G32_UINT = 17,
	RESOURCE_FORMAT_R32G32_SINT = 18,
	RESOURCE_FORMAT_R32G8X24_TYPELESS = 19,
	RESOURCE_FORMAT_D32_FLOAT_S8X24_UINT = 20,
	RESOURCE_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
	RESOURCE_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
	RESOURCE_FORMAT_R10G10B10A2_TYPELESS = 23,
	RESOURCE_FORMAT_R10G10B10A2_UNORM = 24,
	RESOURCE_FORMAT_R10G10B10A2_UINT = 25,
	RESOURCE_FORMAT_R11G11B10_FLOAT = 26,
	RESOURCE_FORMAT_R8G8B8A8_TYPELESS = 27,
	RESOURCE_FORMAT_R8G8B8A8_UNORM = 28,
	RESOURCE_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
	RESOURCE_FORMAT_R8G8B8A8_UINT = 30,
	RESOURCE_FORMAT_R8G8B8A8_SNORM = 31,
	RESOURCE_FORMAT_R8G8B8A8_SINT = 32,
	RESOURCE_FORMAT_R16G16_TYPELESS = 33,
	RESOURCE_FORMAT_R16G16_FLOAT = 34,
	RESOURCE_FORMAT_R16G16_UNORM = 35,
	RESOURCE_FORMAT_R16G16_UINT = 36,
	RESOURCE_FORMAT_R16G16_SNORM = 37,
	RESOURCE_FORMAT_R16G16_SINT = 38,
	RESOURCE_FORMAT_R32_TYPELESS = 39,
	RESOURCE_FORMAT_D32_FLOAT = 40,
	RESOURCE_FORMAT_R32_FLOAT = 41,
	RESOURCE_FORMAT_R32_UINT = 42,
	RESOURCE_FORMAT_R32_SINT = 43,
	RESOURCE_FORMAT_R24G8_TYPELESS = 44,
	RESOURCE_FORMAT_D24_UNORM_S8_UINT = 45,
	RESOURCE_FORMAT_R24_UNORM_X8_TYPELESS = 46,
	RESOURCE_FORMAT_X24_TYPELESS_G8_UINT = 47,
	RESOURCE_FORMAT_R8G8_TYPELESS = 48,
	RESOURCE_FORMAT_R8G8_UNORM = 49,
	RESOURCE_FORMAT_R8G8_UINT = 50,
	RESOURCE_FORMAT_R8G8_SNORM = 51,
	RESOURCE_FORMAT_R8G8_SINT = 52,
	RESOURCE_FORMAT_R16_TYPELESS = 53,
	RESOURCE_FORMAT_R16_FLOAT = 54,
	RESOURCE_FORMAT_D16_UNORM = 55,
	RESOURCE_FORMAT_R16_UNORM = 56,
	RESOURCE_FORMAT_R16_UINT = 57,
	RESOURCE_FORMAT_R16_SNORM = 58,
	RESOURCE_FORMAT_R16_SINT = 59,
	RESOURCE_FORMAT_R8_TYPELESS = 60,
	RESOURCE_FORMAT_R8_UNORM = 61,
	RESOURCE_FORMAT_R8_UINT = 62,
	RESOURCE_FORMAT_R8_SNORM = 63,
	RESOURCE_FORMAT_R8_SINT = 64,
	RESOURCE_FORMAT_A8_UNORM = 65,
	RESOURCE_FORMAT_R1_UNORM = 66,
	RESOURCE_FORMAT_R9G9B9E5_SHAREDEXP = 67,
	RESOURCE_FORMAT_R8G8_B8G8_UNORM = 68,
	RESOURCE_FORMAT_G8R8_G8B8_UNORM = 69,
	RESOURCE_FORMAT_BC1_TYPELESS = 70,
	RESOURCE_FORMAT_BC1_UNORM = 71,
	RESOURCE_FORMAT_BC1_UNORM_SRGB = 72,
	RESOURCE_FORMAT_BC2_TYPELESS = 73,
	RESOURCE_FORMAT_BC2_UNORM = 74,
	RESOURCE_FORMAT_BC2_UNORM_SRGB = 75,
	RESOURCE_FORMAT_BC3_TYPELESS = 76,
	RESOURCE_FORMAT_BC3_UNORM = 77,
	RESOURCE_FORMAT_BC3_UNORM_SRGB = 78,
	RESOURCE_FORMAT_BC4_TYPELESS = 79,
	RESOURCE_FORMAT_BC4_UNORM = 80,
	RESOURCE_FORMAT_BC4_SNORM = 81,
	RESOURCE_FORMAT_BC5_TYPELESS = 82,
	RESOURCE_FORMAT_BC5_UNORM = 83,
	RESOURCE_FORMAT_BC5_SNORM = 84,
	RESOURCE_FORMAT_B5G6R5_UNORM = 85,
	RESOURCE_FORMAT_B5G5R5A1_UNORM = 86,
	RESOURCE_FORMAT_B8G8R8A8_UNORM = 87,
	RESOURCE_FORMAT_B8G8R8X8_UNORM = 88,
	RESOURCE_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
	RESOURCE_FORMAT_B8G8R8A8_TYPELESS = 90,
	RESOURCE_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
	RESOURCE_FORMAT_B8G8R8X8_TYPELESS = 92,
	RESOURCE_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
	RESOURCE_FORMAT_BC6H_TYPELESS = 94,
	RESOURCE_FORMAT_BC6H_UF16 = 95,
	RESOURCE_FORMAT_BC6H_SF16 = 96,
	RESOURCE_FORMAT_BC7_TYPELESS = 97,
	RESOURCE_FORMAT_BC7_UNORM = 98,
	RESOURCE_FORMAT_BC7_UNORM_SRGB = 99,
	RESOURCE_FORMAT_FORCE_UINT = 0xffffffff
};

typedef UINT ResourceBindFlag;
typedef UINT ResourceCPUAccessFlag;

//Vertex buffer data input type. Is the stream of data in the VB meant as
//per vertex or per instance data?
enum ResourceInputType
{
	RESOURCE_INPUT_PER_VERTEX_DATA = 0,
	RESOURCE_INPUT_PER_INSTANCE_DATA = 1
};

//Structure which, as a set, describes the layout of a vertex buffer. Same
//as D3D11_INPUT_ELEMENT_DESC struct. For OpenGL (if we get to it), some of this
//will be ignored!
struct VertexInputSignatureElementDescription
{
	static VertexInputSignatureElementDescription PerVertex(
		const char* semanticName, UINT semanticIndex, 
		ResourceFormat inputFormat, UINT bufferInputSlot,
		UINT alignedByteOffset)
	{
		VertexInputSignatureElementDescription o;

		o.SemanticName = semanticName;
		o.SemanticIndex = semanticIndex;
		o.InputFormat = inputFormat;
		o.BufferInputSlotIndex = bufferInputSlot;
		o.AlignedByteOffset = alignedByteOffset;

		o.InputType = RESOURCE_INPUT_PER_VERTEX_DATA;
		o.InstanceDataStepRate = 0;

		return o;
	}

	static VertexInputSignatureElementDescription PerInstance(
		const char* semanticName, UINT semanticIndex,
		ResourceFormat inputFormat, UINT bufferInputSlot,
		UINT alignedByteOffset, 
		UINT instanceDataStepRate)
	{
		VertexInputSignatureElementDescription o;

		o.SemanticName = semanticName;
		o.SemanticIndex = semanticIndex;
		o.InputFormat = inputFormat;
		o.BufferInputSlotIndex = bufferInputSlot;
		o.AlignedByteOffset = alignedByteOffset;

		o.InputType = RESOURCE_INPUT_PER_INSTANCE_DATA;
		o.InstanceDataStepRate = instanceDataStepRate;

		return o;
	}

	std::string SemanticName;
	UINT SemanticIndex;
	ResourceFormat InputFormat;
	UINT BufferInputSlotIndex;
	UINT AlignedByteOffset;
	ResourceInputType InputType;
	UINT InstanceDataStepRate;
};