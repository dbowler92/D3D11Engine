//D3D11VertexInputSignature.h
//Created 11/07/17
//Created By Daniel Bowler
//
//ID3D11InputLayout. 
//
//TODO: Some form of manager so that we can reuse these rather than create them
//each and every time.

#pragma once

//vector
#include <vector>

//Parent class
#include <Core/CoreObject/CoreObject.h>

//Graphics includes - inc D3D11
#include <Includes/GraphicsIncludes.h>

//Device used to create texture, buffers && views
#include <Graphics/GraphicsDevice/GraphicsDevice.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11VertexInputSignature : public EngineAPI::Core::CoreObject
			{
			public:
				D3D11VertexInputSignature() {};
				virtual ~D3D11VertexInputSignature() = 0 {};

				//Inits the input layout/signature
				bool InitVertexInputSignature(EngineAPI::Graphics::GraphicsDevice* device,
					VertexInputSignatureElementDescription* inputs, uint32_t inputsCount, 
					const char* vertexShaderBytecodeWithSignature, SIZE_T shaderByteCodeSize, 
					std::string ownerDebugName); //We append _VertexInputSignature after!
				
				//Shutsdown the underlying signature object
				void Shutdown();

			public:
				//Gets the input layout to bind to IA stage
				ID3D11InputLayout* GetD3D11InputLayout() { return inputLayout; };

			protected:
				//D3D11 input layout
				ID3D11InputLayout* inputLayout = nullptr;

				//D3D11 understanding of my VertexInputSignatureElementDescription
				std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementsArray;
			};
		};
	};
};
