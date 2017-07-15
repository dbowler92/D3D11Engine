#include "D3D11ShaderResourceView.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11ShaderResourceView::Shutdown()
{
	//Shutdown self
	ReleaseCOM(srv);

	//Shutdown parent
	__super::Shutdown();
}

bool D3D11ShaderResourceView::InitShaderResourceViewToTexture2D(EngineAPI::Graphics::GraphicsDevice* device,
	EngineAPI::Graphics::Texture2D* texture,
	bool doUseUnderlyingResourceFormatForView,
	ResourceFormat renderTargetTextureViewFormat,
	std::string debugName)
{


	//Done
	return true;
}