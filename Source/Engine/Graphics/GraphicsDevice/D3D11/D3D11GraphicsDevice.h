//D3D11GraphicsDevice.h
//Created 09/07/17
//Created By Daniel Bowler
//
//Represents the ID3D11Device && ID3D11DeviceContext

#pragma once

//Parent
#include <Core/CoreObject/CoreObject.h>

//Graphics includes - inc D3D11
#include <Includes/GraphicsIncludes.h>

//Debug
#include <Debug/Log/DebugLog.h>

//Globals
const float DEFAULT_BLEND_FACTOR[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

//Limits
#define MAX_BOUND_SHADER_RESOURCE_VIEWS_PS 128

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
		class HullShader;
		class DomainShader;
		class PixelShader;

		class RasterizerState;
		class BlendState;
		class DepthStencilState;
		class SamplerState;

		class RenderTargetView;
		class DepthStencilView;
		class ShaderResourceView;
	};
	namespace Rendering
	{
		class RenderTarget;
		class RenderTargetSet;
	};
};

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
				void IASetVertexBuffer(EngineAPI::Graphics::VertexBuffer* vb, UINT bufferSlot, UINT offset);
				//void IASetVertexBuffers(EngineAPI::Graphics::VertexBuffer** vbs, UINT bufferCount, UINT* bufferSlots, UINT* offsets);
				void IASetIndexBuffer(EngineAPI::Graphics::IndexBuffer* ib, UINT offset);

				//Rasterizer (RS)
				void RSSetState(EngineAPI::Graphics::RasterizerState* rss);
				void RSSetViewport(Viewport* vp); //Pass null here to set default VP

				//Ouput Merger (OM)
				void OMSetBlendState(EngineAPI::Graphics::BlendState* bs, const float blendFactor[4] = DEFAULT_BLEND_FACTOR, UINT sampleMask = 0xFFFFFFFF);
				void OMSetDepthStencilState(EngineAPI::Graphics::DepthStencilState* dss, UINT stencilRef);

				//OM - Setting render target(s)
				void OMSetRenderTarget(EngineAPI::Graphics::RenderTargetView* renderTargetView,
					EngineAPI::Graphics::DepthStencilView* optionalDepthStencilView);
				void OMSetRenderTargets(uint32_t renderTargetsCount,
					EngineAPI::Rendering::RenderTarget** renderTargetsArray, 
					EngineAPI::Graphics::DepthStencilView* optionalDepthStencilView);

				//VS
				void VSSetShader(EngineAPI::Graphics::VertexShader* vs);
				void VSSetConstantBuffer(EngineAPI::Graphics::ConstantBuffer* cBuffer, UINT bufferSlot);
				void VSSetShaderResource(EngineAPI::Graphics::ShaderResourceView* shaderResource, UINT bindingSlot);
				void VSSetSamplerState(EngineAPI::Graphics::SamplerState* samplerState, UINT bindingSlot);

				//HS
				void HSSetShader(EngineAPI::Graphics::HullShader* hs);

				//DS
				void DSSetShader(EngineAPI::Graphics::DomainShader* ds);
				void DSSetConstantBuffer(EngineAPI::Graphics::ConstantBuffer* cBuffer, UINT bufferSlot);

				//PS
				void PSSetShader(EngineAPI::Graphics::PixelShader* ps);
				void PSSetConstantBuffer(EngineAPI::Graphics::ConstantBuffer* cBuffer, UINT bufferSlot);
				void PSSetShaderResource(EngineAPI::Graphics::ShaderResourceView* shaderResource, UINT bindingSlot);
				void PSSetShaderResources(EngineAPI::Graphics::ShaderResourceView** shaderResources, UINT count, UINT startBindingSlot);
				void PSSetSamplerState(EngineAPI::Graphics::SamplerState* samplerState, UINT bindingSlot);

				//Drawing
				void Draw(UINT count, UINT startIndex);
				void DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation);

				//Mapping
				//bool MapResource();
				//void UnmapResource();

				//Clearing render targets & depth/stencil buffers
				void ClearRenderTarget(EngineAPI::Graphics::RenderTargetView* rtv, Float32Colour clearColour);
				void ClearDepthStencilBuffer(EngineAPI::Graphics::DepthStencilView* dsv,
					DepthStencilClearFlag depthStencilBufferClearFlag = DEPTH_STENCIL_BUFFER_CLEAR_DEPTH_BIT | DEPTH_STENCIL_BUFFER_CLEAR_STENCIL_BIT,
					float depthClear = 1.0f, uint8_t stencilClear = 0);

			protected:
				//D3D11 resources
				ID3D11Device* d3dDevice = nullptr;
				ID3D11DeviceContext* d3dImmediateContext = nullptr;
			};
		}
	};
};
