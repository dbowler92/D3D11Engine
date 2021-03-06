//D3D11BufferResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Common parent class of all D3D11 buffers

#pragma once

//Parent class
#include <Graphics/CommonResource/CommonResource.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11BufferResource : public EngineAPI::Graphics::CommonResource
			{
			public:
				D3D11BufferResource() {};
				virtual ~D3D11BufferResource() = 0 {};

				//Cleansup the buffer resource
				virtual void Shutdown() override = 0;

				//Override the SetDebugName() -> We will also set the
				//debug name of the underlying ID3D11Resource* / ID3D11DeviceChild*
				virtual void SetDebugName(std::string s) override;

				//Override the map and unmap resource functions
				virtual bool MapResource(EngineAPI::Graphics::GraphicsDevice* device,
					UINT subresourceIndex, ResourceMappingMode mapMode, MappedResourceData* mappedResourceOut) override;
				virtual void UnmapResource(EngineAPI::Graphics::GraphicsDevice* device, 
					UINT subresourceIndex) override;

			public:
				//Gets the buffer object
				ID3D11Buffer* GetD3D11Buffer() { return buffer; };
				ID3D11Resource* GetD3D11BufferAsResource() { return (ID3D11Resource*)buffer; };

			protected:
				//Buffer description
				D3D11_BUFFER_DESC bufferDesc = {};
				D3D11_SUBRESOURCE_DATA bufferInitialData = {};

				//Each buffer resource creates an ID3D11Buffer*
				ID3D11Buffer* buffer = nullptr;

			protected:
				//Inits the ID3D11Buffer
				bool InitBuffer(EngineAPI::Graphics::GraphicsDevice* device, bool doInitWitInitialData,
					ResourceType resourceType, ResourceUsage resourceUsage, ResourceCPUAccessFlag cpuAccess, ResourceBindFlag resourceBindingFlag,
					std::string debugName);
			};
		};
	};
};