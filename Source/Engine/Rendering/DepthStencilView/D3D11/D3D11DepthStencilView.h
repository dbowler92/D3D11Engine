//D3D11DepthStencilView.h
//Created 10/07/17
//Created By Daniel Bowler
//
//ID3D11DepthStencilView

#pragma once

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Debug
#include "../../Debug/Log/DebugLog.h"

//Device used to create views
#include "../../Graphics/GraphicsDevice/GraphicsDevice.h"

namespace EngineAPI
{
	namespace Rendering
	{
		class DepthTexture;
	};
};

namespace EngineAPI
{
	namespace Rendering
	{
		namespace Platform
		{
			class D3D11DepthStencilView
			{
			public:
				D3D11DepthStencilView() {};
				virtual ~D3D11DepthStencilView() = 0 {};

				//Inits the D3D11 DSV
				bool InitDepthStencilView(EngineAPI::Graphics::GraphicsDevice* device,
					class EngineAPI::Rendering::DepthTexture* depthTextureResource,
					bool isReadOnlyDSV = false,
					std::string debugName = std::string(""));

				//Shutdown the DSV
				void Shutdown();

			public:
				//Getters
				ID3D11DepthStencilView* GetD3D11DepthStencilView() { return dsv; };

				//Query
				//
				//ReadOnly DSVs can be bound to the OM for depth testing whilst also bound for
				//sampling in the pixel shader (Useful in deferred rendering).
				bool IsReadOnlyDSV() { return (dsvDesc.Flags & D3D11_DSV_READ_ONLY_DEPTH); };

			protected:
				//Debug name
				std::string debugIDString = "";

				//Description
				D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

				//Handle to the DSV
				ID3D11DepthStencilView* dsv = nullptr;
			};
		};
	};
};