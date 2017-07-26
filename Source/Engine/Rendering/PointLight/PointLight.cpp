#include "PointLight.h"

//Graphics manager
#include <Graphics/GraphicsManager/GraphicsManager.h>

//Uses some of the static engine resources
#include <Statics/GraphicsStatics/GraphicsStatics.h>

//Scene camera
#include <Rendering/VirtualCamera/VirtualCamera.h>

using namespace EngineAPI::Rendering;

void PointLight::Shutdown()
{
	//Shutdown self
	isActive = false;

	//Shutdown super
	__super::Shutdown();
}

void PointLight::InitPointLightSource(XMFLOAT3 position, 
	float range, XMFLOAT3 col, float intensity,
	std::string debugName)
{
	//Cache data
	lightData.LightPosition = position;
	lightData.LightRange = range;
	lightData.LightColourAndIntensity = XMFLOAT4(col.x, col.y, col.z, intensity);

	//Set debug name
	SetDebugName(debugName);
}

void PointLight::Render(EngineAPI::Rendering::VirtualCamera* mainCamera)
{
	assert(mainCamera != nullptr);

	if (!isActive)
		return;

	EngineAPI::Graphics::GraphicsDevice* device =
		EngineAPI::Graphics::GraphicsManager::GetInstance()->GetDevice();
	assert(device);

	//Calculate light world * view * projection matrix
	XMMATRIX lightWorld = XMMatrixScaling(lightData.LightRange / 2.f, lightData.LightRange / 2.f, lightData.LightRange / 2.f) * 
		XMMatrixTranslation(lightData.LightPosition.x, lightData.LightPosition.y, lightData.LightPosition.z);
	XMMATRIX lightWorldViewProj = lightWorld * mainCamera->GetView() * mainCamera->GetProj();
	XMStoreFloat4x4(&lightData.LightWorldViewProjection, XMMatrixTranspose(lightWorldViewProj));

	//Write data to the shared pointlight data cbuffer
	EngineAPI::Graphics::ConstantBuffer& cb = EngineAPI::Statics::GraphicsStatics::LightPass_PointLight_LightDataCB;

	MappedResourceData mr = {};
	if (cb.MapResource(device, 0, RESOURCE_MAP_WRITE_DISCARD, &mr))
		memcpy(mr.MappedData, &lightData, sizeof(PointLightGraphicsData));
	cb.UnmapResource(device, 0);

	//Render the light source during the lighting pass
	//
	//Bind the light data CBuffer (Slot 1)
	device->DSSetConstantBuffer(&cb, GRAPHICS_CONFIG_LIGHT_PASS_LIGHT_DATA_CBUFFER_BINDING_SLOT);
	device->PSSetConstantBuffer(&cb, GRAPHICS_CONFIG_LIGHT_PASS_LIGHT_DATA_CBUFFER_BINDING_SLOT);

	//Draw - 2 patches -> One for each side the point light (we generate
	//2 half spheres)
	device->Draw(2, 0);
}