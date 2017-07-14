//D3D11ViewResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Common parent class of all D3D11 view (SRV, DSV etc) objects

#pragma once

//Parent class
#include "../../../Graphics/BaseViewResource/BaseViewResource.h"

//Statics (D3D11)
#include "../../../Statics/D3D11Statics/D3D11Statics.h"

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11ViewResource : public EngineAPI::Graphics::BaseViewResource
			{
			public:
				D3D11ViewResource() {};
				virtual ~D3D11ViewResource() = 0 {};
		
				//Views should implement their own shutdown function - ensuring that
				//they call __super::Shutdown() to shutdown the core object
				virtual void Shutdown() override = 0;

				//Override the SetDebugName() -> We will also set the
				//debug name of the underlying ID3D11Resource* / ID3D11DeviceChild*
				virtual void SetDebugName(std::string s) override;

			protected:
				//D3D11 Interface for each resource view - make sure to set this
				//in the subclasses by calling D3D11ViewResource::CacheD3D11ViewInterface();
				ID3D11View* baseViewInterface = nullptr;

			protected:
				//Setsup the internal baseViewInterface
				void CacheD3D11ViewInterface(ID3D11View* view) { baseViewInterface = view; };
			};
		};
	};
};