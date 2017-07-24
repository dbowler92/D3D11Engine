//D3D11HullShader.h
//Created 24/07/17
//Created By Daniel Bowler
//
//Programmable hull shader. 

#pragma once

#include <Graphics/ShaderResource/ShaderResource.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11HullShader : public EngineAPI::Graphics::ShaderResource
			{
			public:
				D3D11HullShader() {};
				virtual ~D3D11HullShader() = 0 {};

				//Init the HS - This would be a precompiled shader. In D3D11 land,
				//we can compile our HLSL files using FXC (included as part of VS2015)
				bool InitCompiledHullShaderFromFile(EngineAPI::Graphics::GraphicsDevice* device,
					const char* compiledShaderFile,
					std::string debugName = std::string(""));

				//Shutsdown the HS
				virtual void Shutdown() override;

			public:
				//Binds the hull shader to the pipeline
				bool BindHullShaderToPipeline(EngineAPI::Graphics::GraphicsDevice* device);

			protected:
				//Creates and manages a ID3D11HullShader*
				ID3D11HullShader* hullShader = nullptr;
			};
		};
	};
};