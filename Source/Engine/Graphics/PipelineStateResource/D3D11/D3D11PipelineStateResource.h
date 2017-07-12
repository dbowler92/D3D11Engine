//D3D11PipelineStateResource.h
//Created 12/07/17
//Created By Daniel Bowler
//
//Base class for each D3D11 pipeline state object

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
			class D3D11PipelineStateResource : public EngineAPI::Core::CoreObject
			{
			public:
				D3D11PipelineStateResource() {};
				virtual ~D3D11PipelineStateResource() = 0 {};

				//Shutsdown the pipeline state
				virtual void Shutdown() = 0;
			};
		};
	};
};