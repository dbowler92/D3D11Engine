#include "D3D11GraphicsDevice.h"

//Forward declarations
#include <OS/OSWindow/OSWindow.h>

#include <Graphics/VertexBuffer/VertexBuffer.h>
#include <Graphics/IndexBuffer/IndexBuffer.h>

#include <Graphics/ConstantBuffer/ConstantBuffer.h>

#include <Graphics/VertexShader/VertexShader.h>
#include <Graphics/HullShader/HullShader.h>
#include <Graphics/DomainShader/DomainShader.h>
#include <Graphics/PixelShader/PixelShader.h>

#include <Graphics/RasterizerState/RasterizerState.h>
#include <Graphics/BlendState/BlendState.h>
#include <Graphics/DepthStencilState/DepthStencilState.h>
#include <Graphics/SamplerState/SamplerState.h>

#include <Graphics/RenderTargetView/RenderTargetView.h>
#include <Graphics/DepthStencilView/DepthStencilView.h>
#include <Graphics/ShaderResourceView/ShaderResourceView.h>

#include <Rendering/RenderTarget/RenderTarget.h>
#include <Rendering/RenderTargetSet/RenderTargetSet.h>

using namespace EngineAPI::Graphics::Platform;

bool D3D11GraphicsDevice::InitD3D11DeviceAndImmediateContext(EngineAPI::OS::OSWindow* osWindow)
{
	//Set debug name
	SetDebugName("D3D11DeviceAndContext");

	//If debug mode, set the D3D11_CREATE_DEVICE_DEBUG flag
	UINT createDeviceFlags = 0;
#if defined (DEBUG) || defined (_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//Hardware driver type
	D3D_DRIVER_TYPE d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;

	//Feature levels struct to be passed to D3D11CreateDevice()
	D3D_FEATURE_LEVEL featureLevel;

	//Create the device.
	HRESULT hr = D3D11CreateDevice(
		NULL,                 //Default adapter
		d3dDriverType,
		NULL,
		createDeviceFlags,
		NULL, NULL,            //Default feature levels array
		D3D11_SDK_VERSION,
		&d3dDevice,
		&featureLevel,
		&d3dImmediateContext);

	//Check for error
	if (FAILED(hr))
	{
		ReleaseCOM(d3dDevice);
		ReleaseCOM(d3dImmediateContext);

		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11GraphicsDevice::InitD3D11Device() - D3D11CreateDevice() failed!");
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		ReleaseCOM(d3dDevice);
		ReleaseCOM(d3dImmediateContext);

		//Another error
		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11GraphicsDevice::InitD3D11Device() - D3D feature level is not 11_0");
		return false;
	}

	//Done
	return true;
}

void D3D11GraphicsDevice::Shutdown()
{
	ReleaseCOM(d3dImmediateContext);
	ReleaseCOM(d3dDevice);

	//Shutdown core object
	__super::Shutdown();
}

//
//IA
//

void D3D11GraphicsDevice::IASetTopology(PrimitiveTopology topology)
{
	GetD3D11ImmediateContext()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology);
}

void D3D11GraphicsDevice::IASetVertexBuffer(EngineAPI::Graphics::VertexBuffer* vb, UINT bufferSlot, UINT offset)
{
	if (vb)
		vb->BindVertexBufferToPipeline((EngineAPI::Graphics::GraphicsDevice*)this, bufferSlot, offset);
	else
	{
		ID3D11Buffer* buffers = 0;
		UINT nullStrides = 0;
		UINT nullOffsets = 0;
		GetD3D11ImmediateContext()->IASetVertexBuffers(bufferSlot, 1, &buffers, &nullStrides, &nullOffsets);
	}
}

void D3D11GraphicsDevice::IASetIndexBuffer(EngineAPI::Graphics::IndexBuffer* ib, UINT offset)
{
	if (ib)
		ib->BindIndexBufferToPipeline((EngineAPI::Graphics::GraphicsDevice*)this, offset);
	else
		GetD3D11ImmediateContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R16_UINT, 0);
}

//
//RS
//

void D3D11GraphicsDevice::RSSetState(EngineAPI::Graphics::RasterizerState* rss)
{
	if (rss)
		rss->BindRasterizerStateToPipeline((EngineAPI::Graphics::GraphicsDevice*)this);
	else
		GetD3D11ImmediateContext()->RSSetState(nullptr);
}

void D3D11GraphicsDevice::RSSetViewport(Viewport* vp)
{
	if (vp == nullptr)
	{
		//Default VP
		GetD3D11ImmediateContext()->RSSetViewports(0, nullptr);
	}
	else
	{
		//Custom VP
		D3D11_VIEWPORT d3dVP = {};
		d3dVP.TopLeftX = vp->TopLeftX;
		d3dVP.TopLeftY = vp->TopLeftY;

		d3dVP.Width = vp->Width;
		d3dVP.Height = vp->Height;

		d3dVP.MinDepth = vp->MinDepth;
		d3dVP.MaxDepth = vp->MaxDepth;

		GetD3D11ImmediateContext()->RSSetViewports(1, &d3dVP);
	}
}

//
//OM
//

void D3D11GraphicsDevice::OMSetBlendState(EngineAPI::Graphics::BlendState* bs, const float blendFactor[4], UINT sampleMask)
{
	if (bs)
		bs->BindBlendStateToPipeline((EngineAPI::Graphics::GraphicsDevice*)this, blendFactor, sampleMask);
	else
		GetD3D11ImmediateContext()->OMSetBlendState(nullptr, DEFAULT_BLEND_FACTOR, 0xFFFFFFFF);
}

void D3D11GraphicsDevice::OMSetDepthStencilState(EngineAPI::Graphics::DepthStencilState* dss, UINT stencilRef)
{
	if (dss)
		dss->BindDepthStencilStateToPipeline((EngineAPI::Graphics::GraphicsDevice*)this, stencilRef);
	else
		GetD3D11ImmediateContext()->OMSetDepthStencilState(nullptr, 0);
}

void D3D11GraphicsDevice::OMSetRenderTarget(EngineAPI::Graphics::RenderTargetView* renderTargetView,
	EngineAPI::Graphics::DepthStencilView* optionalDepthStencilView)
{
	if (renderTargetView)
		renderTargetView->BindAsRenderTarget((EngineAPI::Graphics::GraphicsDevice*)this, optionalDepthStencilView);
	else
		GetD3D11ImmediateContext()->OMSetRenderTargets(1, nullptr, nullptr);
}

void D3D11GraphicsDevice::OMSetRenderTargets(uint32_t renderTargetsCount,
	EngineAPI::Rendering::RenderTarget** renderTargetsArray,
	EngineAPI::Graphics::DepthStencilView* optionalDepthStencilView)
{
	assert(renderTargetsCount <= MAX_RENDER_TARGETS_BOUND);

	//Build D3D11 array of render targets
	static ID3D11RenderTargetView* renderTargets[MAX_RENDER_TARGETS_BOUND];
	for (uint32_t i = 0; i < renderTargetsCount; i++)
	{
		if (renderTargetsArray != nullptr) //Pass null ptr with renderTargetsCount > 0 == unbind renderTargetsCount render targets from OM
		{
			if (renderTargetsArray[i] != nullptr) //...Can also pass an array of nullptrs -> Same effect as passing a nullptr to renderTargetsArray
			{
				EngineAPI::Graphics::RenderTargetView* rtv = renderTargetsArray[i]->GetRenderTargetView();
				renderTargets[i] = rtv->GetD3D11RenderTargetView();
			}
			else
				renderTargets[i] = nullptr;
		}
		else
		{
			//If we pass a nullptr for renderTargetsArray, this means we want to bind renderTargetCount
			//null render targets (eg: Unbinding GBuffer from OM)
			renderTargets[i] = nullptr;
		}
	}

	//D3D11 depth stencil view
	ID3D11DepthStencilView* dsv = nullptr;
	if (optionalDepthStencilView)
		dsv = optionalDepthStencilView->GetD3D11DepthStencilView();

	//Bind to OM
	GetD3D11ImmediateContext()->OMSetRenderTargets(renderTargetsCount, &renderTargets[0], dsv);
}

//
//VS
//

void D3D11GraphicsDevice::VSSetShader(EngineAPI::Graphics::VertexShader* vs)
{
	if (vs)
		vs->BindVertexShaderToPipeline((EngineAPI::Graphics::GraphicsDevice*)this);
	else
		GetD3D11ImmediateContext()->VSSetShader(nullptr, nullptr, 0);
}

void D3D11GraphicsDevice::VSSetConstantBuffer(EngineAPI::Graphics::ConstantBuffer* cBuffer, UINT bufferSlot)
{
	if (cBuffer)
		cBuffer->BindConstantBufferToVertexShaderStage((EngineAPI::Graphics::GraphicsDevice*)this, bufferSlot);
	else
	{
		ID3D11Buffer* buffers = 0;
		GetD3D11ImmediateContext()->VSSetConstantBuffers(bufferSlot, 1, &buffers);
	}
}

void D3D11GraphicsDevice::VSSetShaderResource(EngineAPI::Graphics::ShaderResourceView* shaderResource, UINT bindingSlot)
{
	if (shaderResource)
		shaderResource->BindShaderResourceViewToVertexShader((EngineAPI::Graphics::GraphicsDevice*)this, bindingSlot);
	else
	{
		ID3D11ShaderResourceView* srvs = 0;
		GetD3D11ImmediateContext()->VSSetShaderResources(bindingSlot, 1, &srvs);
	}
}

void D3D11GraphicsDevice::VSSetSamplerState(EngineAPI::Graphics::SamplerState* samplerState, UINT bindingSlot)
{
	if (samplerState)
		samplerState->BindSamplerStateToVertexShader((EngineAPI::Graphics::GraphicsDevice*)this, bindingSlot);
	else
	{
		ID3D11SamplerState* samplers = 0;
		GetD3D11ImmediateContext()->VSSetSamplers(bindingSlot, 1, &samplers);
	}
}

//
//HS
//

void D3D11GraphicsDevice::HSSetShader(EngineAPI::Graphics::HullShader* hs)
{
	if (hs)
		hs->BindHullShaderToPipeline((EngineAPI::Graphics::GraphicsDevice*)this);
	else
		GetD3D11ImmediateContext()->HSSetShader(nullptr, nullptr, 0);
}

//
//DS
//

void D3D11GraphicsDevice::DSSetShader(EngineAPI::Graphics::DomainShader* ds)
{
	if (ds)
		ds->BindDomainShaderToPipeline((EngineAPI::Graphics::GraphicsDevice*)this);
	else
		GetD3D11ImmediateContext()->DSSetShader(nullptr, nullptr, 0);
}

void D3D11GraphicsDevice::DSSetConstantBuffer(EngineAPI::Graphics::ConstantBuffer* cBuffer, UINT bufferSlot)
{
	if (cBuffer)
		cBuffer->BindConstantBufferToDomainShaderStage((EngineAPI::Graphics::GraphicsDevice*)this, bufferSlot);
	else
	{
		ID3D11Buffer* buffers = 0;
		GetD3D11ImmediateContext()->DSSetConstantBuffers(bufferSlot, 1, &buffers);
	}
}

//
//PS
//

void D3D11GraphicsDevice::PSSetShader(EngineAPI::Graphics::PixelShader* ps)
{
	if (ps)
		ps->BindPixelShaderToPipeline((EngineAPI::Graphics::GraphicsDevice*)this);
	else
		GetD3D11ImmediateContext()->PSSetShader(nullptr, nullptr, 0);
}

void D3D11GraphicsDevice::PSSetConstantBuffer(EngineAPI::Graphics::ConstantBuffer* cBuffer, UINT bufferSlot)
{
	if (cBuffer)
		cBuffer->BindConstantBufferToPixelShaderStage((EngineAPI::Graphics::GraphicsDevice*)this, bufferSlot);
	else
	{
		ID3D11Buffer* buffers = 0;
		GetD3D11ImmediateContext()->PSSetConstantBuffers(bufferSlot, 1, &buffers);
	}
}

void D3D11GraphicsDevice::PSSetShaderResource(EngineAPI::Graphics::ShaderResourceView* shaderResource, UINT bindingSlot)
{
	if (shaderResource)
		shaderResource->BindShaderResourceViewToPixelShader((EngineAPI::Graphics::GraphicsDevice*)this, bindingSlot);
	else
	{
		ID3D11ShaderResourceView* srvs = 0;
		GetD3D11ImmediateContext()->PSSetShaderResources(bindingSlot, 1, &srvs);
	}
}

void D3D11GraphicsDevice::PSSetShaderResources(EngineAPI::Graphics::ShaderResourceView** shaderResources, UINT count, UINT startBindingSlot)
{
	assert(count <= MAX_BOUND_SHADER_RESOURCE_VIEWS_PS);
	assert(count + startBindingSlot <= MAX_BOUND_SHADER_RESOURCE_VIEWS_PS);

	//Build array of SRVs - 1 per count
	static ID3D11ShaderResourceView* shaderResourceViews[MAX_BOUND_SHADER_RESOURCE_VIEWS_PS];
	for (unsigned i = 0; i < count; i++)
	{
		if (shaderResources != nullptr) //If this is NULL, we want to mass unbind SRVs from the PS
		{
			if (shaderResources[i] != nullptr) //Could be an array of nullptrs
				shaderResourceViews[i] = shaderResources[i]->GetD3D11ShaderResourceView();
			else
				shaderResourceViews[i] = nullptr;
		}
		else
			shaderResourceViews[i] = nullptr;
	}

	//Set shader resources
	GetD3D11ImmediateContext()->PSSetShaderResources(startBindingSlot, count, &shaderResourceViews[0]);
}

void D3D11GraphicsDevice::PSSetSamplerState(EngineAPI::Graphics::SamplerState* samplerState, UINT bindingSlot)
{
	if (samplerState)
		samplerState->BindSamplerStateToPixelShader((EngineAPI::Graphics::GraphicsDevice*)this, bindingSlot);
	else
	{
		ID3D11SamplerState* samplers = 0;
		GetD3D11ImmediateContext()->PSSetSamplers(bindingSlot, 1, &samplers);
	}
}

//
//Draw commands
//

void D3D11GraphicsDevice::Draw(UINT count, UINT startIndex)
{
	GetD3D11ImmediateContext()->Draw(count, startIndex);
}

void D3D11GraphicsDevice::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	GetD3D11ImmediateContext()->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

//
//Mapping
//


//Clearning buffers
void D3D11GraphicsDevice::ClearRenderTarget(EngineAPI::Graphics::RenderTargetView* rtv, Float32Colour clearColour)
{
	if (rtv)
		rtv->ClearRenderTargetView((EngineAPI::Graphics::GraphicsDevice*)this, (const float*)&clearColour);
}

void D3D11GraphicsDevice::ClearDepthStencilBuffer(EngineAPI::Graphics::DepthStencilView* dsv,
	DepthStencilClearFlag depthStencilBufferClearFlag, float depthClear, uint8_t stencilClear)
{
	if (dsv)
		dsv->ClearDepthStencilView((EngineAPI::Graphics::GraphicsDevice*)this, depthStencilBufferClearFlag, depthClear, stencilClear);
}