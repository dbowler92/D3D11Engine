#include "D3D11RenderTexture2D.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11RenderTexture2D::Shutdown()
{
	//Shutdown the texture
	__super::Shutdown();
}

bool D3D11RenderTexture2D::InitRenderTexture2D(EngineAPI::Graphics::GraphicsDevice* device,
	uint32_t textureWidth, uint32_t textureHeight, uint32_t msaaSampleCount,
	void* initialData,
	ResourceFormat textureFormat,
	ResourceUsage textureUsage,
	ResourceCPUAccessFlag textureCpuAccess,
	ResourceBindFlag textureBindingFlag,
	std::string debugName)
{
	//DXGI Format - simple cast
	DXGI_FORMAT dxgiTextureFmt = (DXGI_FORMAT)textureFormat;

	//Calculate the usage flag and cpu access flag in D3D11 world
	D3D11_USAGE d3d11Usage = D3D11_USAGE_DEFAULT;
	UINT d3d11CpuAccess = NULL;
	assert(EngineAPI::Statics::D3D11ResourceStatics::CalculateD3D11UsageFlags(textureUsage, textureCpuAccess,
		d3d11Usage, d3d11CpuAccess));

	//Underlying texture description
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = dxgiTextureFmt;

	//MSAA?
	if (msaaSampleCount == 1)
	{
		//No MSAA
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
	}
	else
	{
		uint32_t maxMSAAQuality = 0;
		device->GetD3D11Device()->CheckMultisampleQualityLevels(dxgiTextureFmt, msaaSampleCount, &maxMSAAQuality);
		assert(maxMSAAQuality > 0);

		//Enable MSAA for this render target
		uint32_t msaaQualityLevel = maxMSAAQuality - 1;
		textureDesc.SampleDesc.Count = msaaSampleCount;
		textureDesc.SampleDesc.Quality = msaaQualityLevel;
	}

	textureDesc.Usage = d3d11Usage;
	textureDesc.BindFlags = (UINT)textureBindingFlag;

	//Verify that we created this RenderTexture2D with the RENDER_TARGET binding. If
	//not, add it in rather than crash
	if ( (textureDesc.BindFlags & D3D11_BIND_RENDER_TARGET) == false )
		textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;

	textureDesc.CPUAccessFlags = d3d11CpuAccess;
	textureDesc.MiscFlags = NULL;

	//Initial data
	bool doInitWithInitialData = (initialData != nullptr) ? true : false;
	textureInitialData.pSysMem = initialData;
	textureInitialData.SysMemPitch = 0;
	textureInitialData.SysMemSlicePitch = 0;

	//API agnostic usage info -> This data may have changed from what
	//was passed
	ResourceUsage usg = (ResourceUsage)textureDesc.Usage;
	ResourceCPUAccessFlag cpuAccess = (ResourceCPUAccessFlag)textureDesc.CPUAccessFlags;
	ResourceBindFlag binding = (ResourceBindFlag)textureDesc.BindFlags;

	//Init the underlying texture2D object
	if (!InitTexture2D(device, doInitWithInitialData, RESOURCE_TYPE_RENDER_TARGET_2D,
		usg, cpuAccess, binding, debugName))
		return false;

	//Done
	return true;
}
