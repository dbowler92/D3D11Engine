//D3D11DomainShader.h
//Created 24/07/17
//Created By Daniel Bowler
//
//Represents a domain shader

#pragma once

#include <Graphics/ShaderResource/ShaderResource.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11DomainShader : public EngineAPI::Graphics::ShaderResource
			{
			public:
				D3D11DomainShader() {};
				virtual ~D3D11DomainShader() = 0 {};

				//Init the DS - This would be a precompiled shader. In D3D11 land,
				//we can compile our HLSL files using FXC (included as part of VS2015)
				bool InitCompiledDomainShaderFromFile(EngineAPI::Graphics::GraphicsDevice* device,
					const char* compiledShaderFile,
					std::string debugName = std::string(""));

				//Shutsdown the DS
				virtual void Shutdown() override;

			public:
				//Binds the domain shader to the pipeline
				bool BindDomainShaderToPipeline(EngineAPI::Graphics::GraphicsDevice* device);

			protected:
				//Creates and manages a ID3D11DomainShader*
				ID3D11DomainShader* domainShader = nullptr;
			};
		};
	};
};