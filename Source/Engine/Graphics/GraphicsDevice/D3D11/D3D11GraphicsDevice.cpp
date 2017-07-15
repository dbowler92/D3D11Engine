#include "D3D11GraphicsDevice.h"

//Forward declarations
#include <OS/OSWindow/OSWindow.h>

#include <Graphics/VertexBuffer/VertexBuffer.h>
#include <Graphics/IndexBuffer/IndexBuffer.h>

#include <Graphics/ConstantBuffer/ConstantBuffer.h>

#include <Graphics/VertexShader/VertexShader.h>
#include <Graphics/PixelShader/PixelShader.h>

#include <Graphics/RasterizerState/RasterizerState.h>
#include <Graphics/BlendState/BlendState.h>
#include <Graphics/DepthStencilState/DepthStencilState.h>

#include <Graphics/RenderTargetView/RenderTargetView.h>
#include <Graphics/DepthStencilView/DepthStencilView.h>
#include <Graphics/ShaderResourceView/ShaderResourceView.h>

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

void D3D11GraphicsDevice::IASetVertexBuffer(EngineAPI::Graphics::VertexBuffer* vb, UINT offset)
{
	if (vb)
		vb->BindVertexBufferToPipeline((EngineAPI::Graphics::GraphicsDevice*)this, offset);
	else
		GetD3D11ImmediateContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
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
}

void D3D11GraphicsDevice::VSSetShaderResource(EngineAPI::Graphics::ShaderResourceView* shaderResource, UINT bindingSlot)
{
	if (shaderResource)
		shaderResource->BindShaderResourceViewToVertexShader((EngineAPI::Graphics::GraphicsDevice*)this, bindingSlot);
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

void D3D11GraphicsDevice::PSSetShaderResource(EngineAPI::Graphics::ShaderResourceView* shaderResource, UINT bindingSlot)
{
	if (shaderResource)
		shaderResource->BindShaderResourceViewToPixelShader((EngineAPI::Graphics::GraphicsDevice*)this, bindingSlot);
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