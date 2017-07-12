//D3D11VertexBuffer.h
//Created 10/07/17
//Created By Daniel Bowler
//
//D3D11 Buffer object holding per vertex && per instance data.

#pragma once

//Parent class
#include "../../Graphics/BufferResource/BufferResource.h"

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11VertexBuffer : public EngineAPI::Graphics::BufferResource
			{
			public:
				D3D11VertexBuffer() {};
				virtual ~D3D11VertexBuffer() = 0 {};

				//Init the VB
				bool InitVertexBuffer(EngineAPI::Graphics::GraphicsDevice* device, 
					uint32_t bufferSizeBytes, void* initialData,
					ResourceUsage usage = RESOURCE_USAGE_IMMUTABLE, 
					ResourceCPUAccessFlag cpuAccess = NULL,
					ResourceBindFlag resourceBinding = RESOURCE_BIND_VERTEX_BUFFER_BIT,
					std::string debugName = std::string(""));

				//Shutsdown the VB and super classes
				virtual void Shutdown() override;

			public:
				//Bind this vertex buffer to the pipeline (on its own)
				void BindVertexBufferToPipeline(EngineAPI::Graphics::GraphicsDevice* device, 
					UINT stride, UINT offset = 0);
			};
		};
	};
};