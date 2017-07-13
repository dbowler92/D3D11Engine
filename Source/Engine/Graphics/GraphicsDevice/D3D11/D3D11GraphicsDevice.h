//D3D11GraphicsDevice.h
//Created 09/07/17
//Created By Daniel Bowler
//
//Represents the ID3D11Device && ID3D11DeviceContext

#pragma once

//Parent
#include "../../Core/CoreObject/CoreObject.h"

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Debug
#include "../../Debug/Log/DebugLog.h"

namespace EngineAPI
{
	namespace OS
	{
		class OSWindow;
	};
	namespace Graphics
	{
		class BufferResource;
		class TextureResource;

		class VertexBuffer;
		class IndexBuffer;

		class ConstantBuffer;

		class VertexShader;
		class PixelShader;

		class RasterizerState;
		class DepthStencilState;
	}
}

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11GraphicsDevice : public EngineAPI::Core::CoreObject
			{
			public:
				D3D11GraphicsDevice() {};
				virtual ~D3D11GraphicsDevice() = 0 {};

				//Inits && shutdown the D3D11 device
				bool InitD3D11DeviceAndImmediateContext(EngineAPI::OS::OSWindow* osWindow); //TODO: Allow user to create deferred context + separate device and context creation + classes
				virtual void Shutdown() override;

				//Returns the device and context
				ID3D11Device* GetD3D11Device() { return d3dDevice; };
				ID3D11DeviceContext* GetD3D11ImmediateContext() { return d3dImmediateContext; };

			public:
				//
				//Interface to the pipeline
				//
				//Input Assembly (IA)
				void IASetTopology(PrimitiveTopology topology);
				void IASetVertexBuffer(EngineAPI::Graphics::VertexBuffer* vb, UINT offset);
				//void IASetVertexBuffers(EngineAPI::Graphics::VertexBuffer* vbs, uint32_t buffCount); //TOOD
				void IASetIndexBuffer(EngineAPI::Graphics::IndexBuffer* ib, UINT offset);

				//Rasterizer (RS)
				void RSSetState(EngineAPI::Graphics::RasterizerState* rss);

				//Ouput Merger (OM)
				void OMSetDepthStencilState(EngineAPI::Graphics::DepthStencilState* dss, UINT stencilRef);

				//VS
				void VSBindShader(EngineAPI::Graphics::VertexShader* vs);
				void VSBindConstantBuffer(EngineAPI::Graphics::ConstantBuffer* cBuffer, UINT bufferSlot);

				//PS
				void PSBindShader(EngineAPI::Graphics::PixelShader* ps);
				
				//Drawing
				void Draw(UINT count, UINT startIndex);
				void DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation);

				//Mapping
				bool MapBufferResource(EngineAPI::Graphics::BufferResource* resource, 
					UINT subresourceIndex, ResourceMappingMode mapMode, MappedResourceData* mappedResourceOut);
				//MappedResourceData MapTextureResource(EngineAPI::Graphics::TextureResource* resource);

				void UnmapBufferResource(EngineAPI::Graphics::BufferResource* resource);
				//void UnmapTextureResource(EngineAPI::Graphics::TextureResource* resource);

			protected:
				//D3D11 resources
				ID3D11Device* d3dDevice = nullptr;
				ID3D11DeviceContext* d3dImmediateContext = nullptr;
			};
		}
	};
};
