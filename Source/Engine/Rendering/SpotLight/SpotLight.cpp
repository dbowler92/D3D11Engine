#include "SpotLight.h"

//Graphics manager
#include <Graphics/GraphicsManager/GraphicsManager.h>

//Uses some of the static engine resources
#include <Statics/GraphicsStatics/GraphicsStatics.h>

//Scene camera
#include <Rendering/VirtualCamera/VirtualCamera.h>

using namespace EngineAPI::Rendering;

void SpotLight::Shutdown()
{
	//Shutdown self
	isActive = false;

	//Shutdown super
	__super::Shutdown();
}

void SpotLight::InitSpotLightSource(XMFLOAT3 position, float range,
	XMFLOAT3 dir, float angle,
	XMFLOAT3 col, float intensity,
	std::string debugName)
{
	//Normalize direction
	XMVECTOR d = XMLoadFloat3(&dir);
	d = XMVector3Normalize(d);
	XMFLOAT3 dNormalized;
	XMStoreFloat3(&dNormalized, d);

	//Cache data
	lightData.LightPosition = position;
	lightData.LightRange = range;
	lightData.LightColourAndIntensity = XMFLOAT4(col.x, col.y, col.z, intensity);
	lightData.LightDirection = dNormalized;
	lightData.LightAngle = angle;

	//Set debug name
	SetDebugName(debugName);
}

void SpotLight::Render(EngineAPI::Rendering::VirtualCamera* mainCamera)
{
	assert(mainCamera != nullptr);

	if (!isActive)
		return;

	EngineAPI::Graphics::GraphicsDevice* device =
		EngineAPI::Graphics::GraphicsManager::GetInstance()->GetDevice();
	assert(device);

	//Calculate light world * view * projection matrix
	//XMMATRIX lightWorld = XMMatrixScaling(lightData.LightRange, lightData.LightRange, lightData.LightRange) * 
	//	XMMatrixTranslation(lightData.LightPosition.x, lightData.LightPosition.y, lightData.LightPosition.z);
	XMMATRIX lightWorld = XMMatrixIdentity(); //TEMP
	XMMATRIX lightWorldViewProj = lightWorld * mainCamera->GetView() * mainCamera->GetProj();
	XMStoreFloat4x4(&lightData.LightWorldViewProjection, XMMatrixTranspose(lightWorldViewProj));
	  
	//Write data to the shared spotlight data cbuffer
	EngineAPI::Graphics::ConstantBuffer& cb = EngineAPI::Statics::GraphicsStatics::LightPass_SpotLight_LightDataCB;

	MappedResourceData mr = {};
	if (cb.MapResource(device, 0, RESOURCE_MAP_WRITE_DISCARD, &mr))
		memcpy(mr.MappedData, &lightData, sizeof(SpotLightGraphicsData));
	cb.UnmapResource(device, 0);

	//Render the light source during the lighting pass
	//
	//Bind the light data CBuffer (Slot 1)
	device->DSSetConstantBuffer(&cb, GRAPHICS_CONFIG_LIGHT_PASS_LIGHT_DATA_CBUFFER_BINDING_SLOT);
	device->PSSetConstantBuffer(&cb, GRAPHICS_CONFIG_LIGHT_PASS_LIGHT_DATA_CBUFFER_BINDING_SLOT);

	//Draw - 1 patch. This one patch contains 1 control point
	device->Draw(1, 0);
}