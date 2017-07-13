#include "D3D11GraphicsDevice.h"

//Forward declarations
#include "../../OS/OSWindow/OSWindow.h"

#include "../../VertexBuffer/VertexBuffer.h"
#include "../../IndexBuffer/IndexBuffer.h"

#include "../../ConstantBuffer/ConstantBuffer.h"

#include "../../VertexShader/VertexShader.h"
#include "../../PixelShader/PixelShader.h"

#include "../../RasterizerState/RasterizerState.h"
#include "../../DepthStencilState/DepthStencilState.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11GraphicsDevice::InitD3D11DeviceAndImmediateContext(EngineAPI::OS::OSWindow* osWindow)
{
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

void D3D11GraphicsDevice::OMSetDepthStencilState(EngineAPI::Graphics::DepthStencilState* dss, UINT stencilRef)
{
	if (dss)
		dss->BindDepthStencilStateToPipeline((EngineAPI::Graphics::GraphicsDevice*)this, stencilRef);
	else
		GetD3D11ImmediateContext()->OMSetDepthStencilState(nullptr, 0);
}

//
//VS
//

void D3D11GraphicsDevice::VSBindShader(EngineAPI::Graphics::VertexShader* vs)
{
	if (vs)
		vs->BindVertexShaderToPipeline((EngineAPI::Graphics::GraphicsDevice*)this);
	else
		GetD3D11ImmediateContext()->VSSetShader(nullptr, nullptr, 0);
}

void D3D11GraphicsDevice::VSBindConstantBuffer(EngineAPI::Graphics::ConstantBuffer* cBuffer, UINT bufferSlot)
{
	if (cBuffer)
		cBuffer->BindConstantBufferToVertexShaderStage((EngineAPI::Graphics::GraphicsDevice*)this, bufferSlot);
}

//
//PS
//

void D3D11GraphicsDevice::PSBindShader(EngineAPI::Graphics::PixelShader* ps)
{
	if (ps)
		ps->BindPixelShaderToPipeline((EngineAPI::Graphics::GraphicsDevice*)this);
	else
		GetD3D11ImmediateContext()->PSSetShader(nullptr, nullptr, 0);
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

bool D3D11GraphicsDevice::MapBufferResource(EngineAPI::Graphics::BufferResource* resource,
	UINT subresourceIndex, ResourceMappingMode mapMode, MappedResourceData* mappedResourceOut)
{
	assert(resource);

	//From api agnostic to D3D11:;
	D3D11_MAP mode = (D3D11_MAP)mapMode;

	//TODO:
	D3D11_MAP_FLAG mapFlag = (D3D11_MAP_FLAG)0;

	//
	//Check if possible
	//

	//Map
	D3D11_MAPPED_SUBRESOURCE mappedRes = {};
	HR(GetD3D11ImmediateContext()->Map(resource->GetD3D11BufferAsResource(), subresourceIndex, mode, mapFlag, &mappedRes));
	if (mappedRes.pData == nullptr)
		return false;

	//Return 
	*mappedResourceOut = {};
	mappedResourceOut->MappedData = mappedRes.pData;
	mappedResourceOut->RowPitch = mappedRes.RowPitch;
	mappedResourceOut->DepthPitch = mappedRes.DepthPitch;
	
	return true;
}

void D3D11GraphicsDevice::UnmapBufferResource(EngineAPI::Graphics::BufferResource* resource)
{
	assert(resource);

	GetD3D11ImmediateContext()->Unmap(resource->GetD3D11BufferAsResource(), 0);
}