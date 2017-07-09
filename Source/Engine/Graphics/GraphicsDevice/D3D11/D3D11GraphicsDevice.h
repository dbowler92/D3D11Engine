//D3D11GraphicsDevice.h
//Created 09/07/17
//Created By Daniel Bowler
//
//Represents the ID3D11Device && ID3D11DeviceContext

#pragma once

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Debug
#include "../../Debug/Log/DebugLog.h"

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11GraphicsDevice
			{
			public:
				D3D11GraphicsDevice() {};
				virtual ~D3D11GraphicsDevice() = 0 {};

				//Inits && shutdown the D3D11 device
				bool InitD3D11DeviceAndContext(); //TODO: Allow user to create deferred context + separate device and context creation + classes
				void ShutdownD3D11DeviceAndContext();

				//Returns the device and context
				ID3D11Device* GetID3D11Device() { return d3dDevice; };
				ID3D11DeviceContext* GetID3D11Context() { return d3dImmediateContext; };

			protected:
				//D3D11 resources
				ID3D11Device* d3dDevice = nullptr;
				ID3D11DeviceContext* d3dImmediateContext = nullptr;
			};
		}
	};
};
