//D3D11GeometryShader.h
//Created 24/07/17
//Created By Daniel Bowler
//
//Represents a geometry shader

#pragma once

#include <Graphics/ShaderResource/ShaderResource.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11GeometryShader : public EngineAPI::Graphics::ShaderResource
			{
			public:
				D3D11GeometryShader() {};
				virtual ~D3D11GeometryShader() = 0 {};

				//Init the GS - This would be a precompiled shader. In D3D11 land,
				//we can compile our HLSL files using FXC (included as part of VS2015)
				bool InitCompiledGeometryShaderFromFile(EngineAPI::Graphics::GraphicsDevice* device,
					const char* compiledShaderFile,
					std::string debugName = std::string(""));

				//Shutsdown the GS
				virtual void Shutdown() override;

			public:
				//Binds the geometry shader to the pipeline
				bool BindHullShaderToPipeline(EngineAPI::Graphics::GraphicsDevice* device);

			protected:
				//Creates and manages a ID3D11GeometryShader*
				ID3D11GeometryShader* geometryShader = nullptr;
			};
		};
	};
};