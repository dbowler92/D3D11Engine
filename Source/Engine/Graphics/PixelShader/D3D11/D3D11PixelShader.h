//D3D11PixelShader.h
//Created 11/07/17
//Created By Daniel Bowler
//
//D3D11 vertex shader loaded from file - will be pre-compiled

#pragma once

//Parent class
#include "../../Graphics/ShaderResource/ShaderResource.h"

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11PixelShader : public EngineAPI::Graphics::ShaderResource
			{
			public:
				D3D11PixelShader() {};
				virtual ~D3D11PixelShader() = 0 {};

				//Init the PS - This would be a precompiled shader. In D3D11 land,
				//we can compile our HLSL files using FXC (included as part of VS2015)
				bool InitCompiledPixelShaderFromFile(EngineAPI::Graphics::GraphicsDevice* device,
					const char* compiledShaderFile,
					std::string debugName = std::string(""));

				//Shutsdown the PS
				virtual void Shutdown() override;

			protected:
				//Creates and manages a ID3D11PixelShader*
				ID3D11PixelShader* pixelShader = nullptr;
			};
		};
	};
};