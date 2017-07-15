#include "D3D11TextureFile2D.h"

//DDSLoader
#include <3rdParty/DDSTextureLoader/DDSTextureLoader.h>

using namespace EngineAPI::Graphics::Platform;

void D3D11TextureFile2D::Shutdown()
{
	//Shutdown tex
	__super::Shutdown();
}

bool D3D11TextureFile2D::InitTexture2DFromDDSFile(EngineAPI::Graphics::GraphicsDevice* device,
	std::string ddsFilename,
	ResourceUsage textureUsage,
	ResourceCPUAccessFlag textureCpuAccess,
	ResourceBindFlag textureBindFlag,
	std::string debugName)
{


	//Done
	return true;
}