//D3D11BufferResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Common parent class of all D3D11 buffers

#pragma once

//Parent class
#include "../../Graphics/CommonResource/CommonResource.h"

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

				//Inits the ID3D11Buffer
				bool InitBuffer(EngineAPI::Graphics::GraphicsDevice* device,
					const D3D11_SUBRESOURCE_DATA* initialData,
					std::string debugName);

				//Cleansup the buffer resource
				virtual void Shutdown() = 0;

			protected:
				//Buffer description
				D3D11_BUFFER_DESC bufferDesc = {};

				//Each buffer resource creates an ID3D11Buffer*
				ID3D11Buffer* buffer = nullptr;
			};
		};
	};
};