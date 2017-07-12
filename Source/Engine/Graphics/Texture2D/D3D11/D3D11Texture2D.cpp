#include "D3D11Texture2D.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11Texture2D::InitTexture2D(EngineAPI::Graphics::GraphicsDevice* device, 
	D3D11_SUBRESOURCE_DATA* textureInitData, std::string debugName)
{
	//Destroy old texture
	if (texture2DHandle)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old texture: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(texture2DHandle);
	}

	//Store debug name
	SetDebugName(debugName);

	//Print message saying we are creating a texture
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Texture: " + GetDebugName();
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create the texture object
	HR(device->GetD3D11Device()->CreateTexture2D(&textureDesc, textureInitData, &texture2DHandle));
	if (texture2DHandle == nullptr)
		return false;

	//Debug name
	EngineAPI::Statics::D3D11ResourceStatics::SetD3D11ResourceDebugName(texture2DHandle, debugName);

	//Done
	return true;
}

void D3D11Texture2D::Shutdown()
{
	//Release texture
	ReleaseCOM(texture2DHandle);
}