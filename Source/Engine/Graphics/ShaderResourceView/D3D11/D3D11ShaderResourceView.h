//D3D11ShaderResourceView.h
//Created 15/07/17
//Created By Daniel Bowler
//
//ID3D11ShaderResourceView*

#pragma once

//Parent class
#include <Graphics/ViewResource/ViewResource.h>

//Graphics includes - inc D3D11
#include <Includes/GraphicsIncludes.h>

//Debug
#include <Debug/Log/DebugLog.h>

//Device used to create views
#include <Graphics/GraphicsDevice/GraphicsDevice.h>

namespace EngineAPI
{
	namespace Graphics
	{
		class Texture2D;
		class RenderTexture2D;
		class DepthStencilTexture2D;
	};
};

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11ShaderResourceView : public EngineAPI::Graphics::ViewResource
			{
			public:
				D3D11ShaderResourceView() {};
				virtual ~D3D11ShaderResourceView() = 0 {};

				//Inits the D3D11 SRV
				bool InitShaderResourceViewToTexture2D(EngineAPI::Graphics::GraphicsDevice* device,
					EngineAPI::Graphics::Texture2D* texture,
					bool doUseUnderlyingResourceFormatForView = true,
					ResourceFormat shaderResourceViewTextureFormat = RESOURCE_FORMAT_USE_UNDERLYING_RESOURCE_FORMAT,
					bool doUseFirstMipOnly = false,				//Else, will use all mip levels. TODO: System to specify a subset of mips to use. 
					std::string debugName = std::string(""));

				//Shutdown the SRV
				virtual void Shutdown() override;

			public:
				//Binds the SRV to the pipeline - just this SRV
				void BindShaderResourceViewToVertexShader(EngineAPI::Graphics::GraphicsDevice* device, UINT bindingSlotIndex);
				void BindShaderResourceViewToPixelShader(EngineAPI::Graphics::GraphicsDevice* device, UINT bindingSlotIndex);

				//Use this SRV to auto generate mips
				bool AutoGenerateMipmaps(EngineAPI::Graphics::GraphicsDevice* device);

			public:
				//Getters
				ID3D11ShaderResourceView* GetD3D11ShaderResourceView() { return srv; };

			protected:
				//Description.
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

				//SRV handle
				ID3D11ShaderResourceView* srv = nullptr;
			};
		};
	};
};
