//D3D11DepthTexture.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Represents a D3D11 depth texture

#pragma once

namespace EngineAPI
{
	namespace Rendering
	{
		namespace Platform
		{
			class D3D11DepthTexture
			{
			public:
				D3D11DepthTexture() {};
				virtual ~D3D11DepthTexture() = 0 {};

				//Inits the depth texture
				bool InitD3D11DepthTexture();

				//Shutsdown the depth texture
				void ShutdownD3D11DepthTexture();
			};
		}
	};
};