//D3D11ViewResource.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Common parent class of all D3D11 view (SRV, DSV etc) objects

#pragma once

//Parent class
#include "../../../Core/CoreObject/CoreObject.h"

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Device used to create texture, buffers && views
#include "../../Graphics/GraphicsDevice/GraphicsDevice.h"

//Statics (D3D11)
#include "../../../Statics/D3D11Statics/D3D11Statics.h"

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11ViewResource : public EngineAPI::Core::CoreObject
			{
			public:
				D3D11ViewResource() {};
				virtual ~D3D11ViewResource() = 0 {};

				//Views should implement their own shutdown function - ensuring that
				//they call __super::Shutdown() to shutdown the core object
				virtual void Shutdown() override = 0;
			};
		};
	};
};