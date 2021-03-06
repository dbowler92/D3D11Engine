//D3D11VertexShader.h
//Created 11/07/17
//Created By Daniel Bowler
//
//D3D11 vertex shader loaded from file - will be pre-compiled

#pragma once

//Parent class
#include <Graphics/ShaderResource/ShaderResource.h>

//Input layout/signature
#include <Graphics/VertexInputSignature/VertexInputSignature.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11VertexShader : public EngineAPI::Graphics::ShaderResource
			{
			public:
				D3D11VertexShader() {};
				virtual ~D3D11VertexShader() = 0 {};

				//Init the VS - This would be a precompiled shader. In D3D11 land,
				//we can compile our HLSL files using FXC (included as part of VS2015)
				bool InitCompiledVertexShaderFromFile(EngineAPI::Graphics::GraphicsDevice* device,
					const char* compiledShaderFile,
					VertexInputSignatureElementDescription* inputSignature, uint32_t inputsCount,
					std::string debugName = std::string(""));

				//Shutsdown the VS
				virtual void Shutdown() override;

			public:
				//Binds the vertex shader to the pipeline
				bool BindVertexShaderToPipeline(EngineAPI::Graphics::GraphicsDevice* device);

			protected:
				//Creates and manages a ID3D11VertexShader*
				ID3D11VertexShader* vertexShader = nullptr;

				//Input signature - creates and manages a ID3D11InputLayout*
				EngineAPI::Graphics::VertexInputSignature inputLayout;
			};
		};
	};
};