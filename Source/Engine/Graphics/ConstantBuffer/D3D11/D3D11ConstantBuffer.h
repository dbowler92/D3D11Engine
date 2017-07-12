//D3D11ConstantBuffer.h
//Created 12/07/17
//Created By Daniel Bowler
//
//D3D11 buffer which will be bound to a shader stage of the pipeline. 

#pragma once

//Parent class
#include "../../Graphics/BufferResource/BufferResource.h"

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11ConstantBuffer : public EngineAPI::Graphics::BufferResource
			{
			public:
				D3D11ConstantBuffer() {};
				virtual ~D3D11ConstantBuffer() = 0 {};

				//Init the CB
				bool InitConstantBuffer(EngineAPI::Graphics::GraphicsDevice* device,
					uint32_t constantBufferSizeBytes, void* initialData,
					ResourceUsage usage = RESOURCE_USAGE_DYNAMIC,  //CPU Writable
					ResourceCPUAccessFlag cpuAccess = RESOURCE_CPU_ACCESS_WRITE_BIT,
					ResourceBindFlag resourceBinding = RESOURCE_BIND_CONSTANT_BUFFER_BIT,
					std::string debugName = std::string(""));

				//Shutsdown the CB and super classes
				virtual void Shutdown() override;

			protected:
				//Cached data
				uint32_t cBufferSizeBytes = 0;

			};
		};
	};
};