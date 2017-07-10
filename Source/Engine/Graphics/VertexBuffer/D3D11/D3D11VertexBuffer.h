//D3D11VertexBuffer.h
//Created 10/07/17
//Created By Daniel Bowler
//
//D3D11 Buffer object holding per vertex && per instance data.

#pragma once

//Parent class
#include "../../Graphics/BufferResource/BufferResource.h"

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Device used to create texture && views
#include "../../Graphics/GraphicsDevice/GraphicsDevice.h"

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
				~D3D11VertexBuffer() = 0 {};
			};
		};
	};
};