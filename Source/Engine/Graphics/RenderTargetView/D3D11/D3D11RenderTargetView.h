//D3D11RenderTargetView.h
//Created 10/04/17
//Created By Daniel Bowler
//
//ID3D11RenderTargetView

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
		class RenderTexture2D;
		class DepthStencilView;
		namespace Platform
		{
			class D3D11GraphicsSwapchain;
		};
	};
};

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11RenderTargetView : public EngineAPI::Graphics::ViewResource
			{
				//Swpachain is a friend so it can access the protected/private
				//InitRenderTargetViewDirectFromD3D11Texture2D() function.
				friend class EngineAPI::Graphics::Platform::D3D11GraphicsSwapchain;

			public:
				D3D11RenderTargetView() {};
				virtual ~D3D11RenderTargetView() = 0 {};
							
				//Inits the D3D11 RTV
				bool InitRenderTargetView(EngineAPI::Graphics::GraphicsDevice* device,
					EngineAPI::Graphics::RenderTexture2D* renderTexture,
					std::string debugName = std::string("")); //TODO

				//Shutdown the RTV
				virtual void Shutdown() override;

			public:
				//Clears this RTV
				void ClearRenderTargetView(EngineAPI::Graphics::GraphicsDevice* device, const float* col);

				//Binds this RTV to the output merger (OM) stage of the pipeline. Will also bind
				//a depth stencil view if provided. 
				//
				//Note: Doesnt bind an array of RTV's (eg: Geometry pass in deferred rendering) - this
				//is comming later. 
				void BindAsRenderTarget(EngineAPI::Graphics::GraphicsDevice* device, 
					EngineAPI::Graphics::DepthStencilView* optionalDSV);

			public:
				//Getters
				ID3D11RenderTargetView* GetD3D11RenderTargetView() { return rtv; };

			protected:
				//Description. NOTE: Empty if this was inited though InitRenderTargetViewDirectFromD3D11Texture2D
				//since we use a null description when calling device->CreateRenderTargetView(...)
				D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};

				//RTV handle
				ID3D11RenderTargetView* rtv = nullptr;

			protected:
				//Special init when creating a RTV to a swapchain texture/buffer -> We don't manually
				//create the resource through our own systems. Therefore, use this function and
				//pass the backbuffer resource direcrtly.
				bool InitRenderTargetViewDirectFromD3D11Texture2D(EngineAPI::Graphics::GraphicsDevice* device,
					ID3D11Texture2D* d3d11TextureResource,
					std::string debugName = std::string(""));
			};
		};
	};
};
