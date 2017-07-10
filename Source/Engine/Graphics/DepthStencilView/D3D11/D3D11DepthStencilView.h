//D3D11DepthStencilView.h
//Created 10/07/17
//Created By Daniel Bowler
//
//ID3D11DepthStencilView

#pragma once

//Parent class
#include "../../Graphics/ViewResource/ViewResource.h"

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Debug
#include "../../Debug/Log/DebugLog.h"

//Device used to create views
#include "../../Graphics/GraphicsDevice/GraphicsDevice.h"

namespace EngineAPI
{
	namespace Graphics
	{
		class DepthTexture2D;
	};
};

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11DepthStencilView : public EngineAPI::Graphics::ViewResource
			{
			public:
				D3D11DepthStencilView() {};
				virtual ~D3D11DepthStencilView() = 0 {};

				//Inits the D3D11 DSV
				bool InitDepthStencilView(EngineAPI::Graphics::GraphicsDevice* device,
					class EngineAPI::Graphics::DepthTexture2D* depthTextureResource,
					bool isReadOnlyDSV = false,
					std::string debugName = std::string(""));

				//Shutdown the DSV
				void Shutdown();

			public:
				//Clears the depth/stencil view/buffer
				bool ClearDepthStencil(EngineAPI::Graphics::GraphicsDevice* device, 
					UINT clearFlag, float depthClear = 1.0f, uint8_t stencilClear = 0);

			public:
				//Getters
				ID3D11DepthStencilView* GetD3D11DepthStencilView() { return dsv; };

				//Query
				//
				//ReadOnly DSVs can be bound to the OM for depth testing whilst also bound for
				//sampling in the pixel shader (Useful in deferred rendering).
				bool IsReadOnlyDSV() { return (dsvDesc.Flags & D3D11_DSV_READ_ONLY_DEPTH); };

			protected:
				//Description
				D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

				//Handle to the DSV
				ID3D11DepthStencilView* dsv = nullptr;
			};
		};
	};
};