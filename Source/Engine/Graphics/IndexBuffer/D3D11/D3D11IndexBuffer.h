//D3D11IndexBuffer.h
//Created 12/07/17
//Created By Daniel Bowler
//
//Represents a D3D11 index buffer - either UINT16 or UINT32

#pragma once

//Parent class
#include <Graphics/BufferResource/BufferResource.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11IndexBuffer : public EngineAPI::Graphics::BufferResource
			{
			public:
				D3D11IndexBuffer() {};
				virtual ~D3D11IndexBuffer() = 0 {};

				//Init the IB
				bool InitIndexBuffer(EngineAPI::Graphics::GraphicsDevice* device,
					IndexBufferFormat indexBufferFormat, uint32_t indexCount,
					void* indexData,
					ResourceUsage usage = RESOURCE_USAGE_IMMUTABLE,
					ResourceCPUAccessFlag cpuAccess = NULL,
					ResourceBindFlag resourceBinding = RESOURCE_BIND_INDEX_BUFFER_BIT,
					std::string debugName = std::string(""));

				//Shutsdown the IB and super classes
				virtual void Shutdown() override;

				//Getters
				inline uint32_t GetIndexCount() { return indexCount; };
				inline IndexBufferFormat GetIndexBufferFormat() { return indexBufferFormat; };
				inline uint8_t  CalculateIndexBufferElementBitsSize() { return (indexBufferFormat == INDEX_BUFFER_FORMAT_UINT16) ? 16 : 32; };
				inline uint8_t  CalculateIndexBufferElementBytesSize() { return (indexBufferFormat == INDEX_BUFFER_FORMAT_UINT16) ? 2 : 4; };
				inline uint32_t CalculateIndexBufferSizeBytes() { return (CalculateIndexBufferElementBytesSize() * GetIndexCount()); };

			public:
				//Bind this index buffer to the pipeline
				void BindIndexBufferToPipeline(EngineAPI::Graphics::GraphicsDevice* device, 
					UINT offset);

			protected:
				//Number of index elements in this buffer
				uint32_t indexCount = 0;

				//Bits per element
				IndexBufferFormat indexBufferFormat;
			};
		};
	};
};