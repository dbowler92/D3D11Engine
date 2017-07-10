//D3D11ViewResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Common parent class of all D3D11 view (SRV, DSV etc) objects

#pragma once

//Parent class
#include "../../Graphics/CommonResource/CommonResource.h"

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11ViewResource : public EngineAPI::Graphics::CommonResource
			{
			public:
				D3D11ViewResource() {};
				virtual ~D3D11ViewResource() = 0 {};
			};
		};
	};
};