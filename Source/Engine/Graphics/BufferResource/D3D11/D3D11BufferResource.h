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
			};
		};
	};
};