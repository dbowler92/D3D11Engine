#include "DirectionalLight.h"

//Graphics manager
#include <Graphics/GraphicsManager/GraphicsManager.h>

//Uses some of the static engine resources
#include <Statics/GraphicsStatics/GraphicsStatics.h>

using namespace EngineAPI::Rendering;

//World space forward vector - 0 rotations will generate a vector
//in this direction
const XMFLOAT4 DIRECTIONAL_LIGHT_FORWARD_VECTOR4 = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

void DirectionalLight::Shutdown()
{
	//Shutdown self
	isActive = false;

	//Shutdown super
	__super::Shutdown();
}

void DirectionalLight::InitDirectionalLightSource(XMFLOAT3 rotationTransformRollPitchYawDegrees,
	XMFLOAT3 col, float intensity,
	std::string debugName)
{
	//Cache rotation
	lightRotationRollPitchYawDegrees = rotationTransformRollPitchYawDegrees;

	//Calculate direction from rotation
	XMVECTOR dir = XMLoadFloat4(&DIRECTIONAL_LIGHT_FORWARD_VECTOR4);
	XMMATRIX dirRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(lightRotationRollPitchYawDegrees.x),
		XMConvertToRadians(lightRotationRollPitchYawDegrees.y),
		XMConvertToRadians(lightRotationRollPitchYawDegrees.z));
	dir = XMVector4Transform(dir, dirRot);

	//Normalize direction
	dir = XMVector3Normalize(dir);
	XMFLOAT3 dNormalized;
	XMStoreFloat3(&dNormalized, dir);

	//Cache data
	lightData.LightDirection = XMFLOAT4(dNormalized.x, dNormalized.y, dNormalized.z, 0.0f);
	lightData.LightColourAndIntensity = XMFLOAT4(col.x, col.y, col.z, intensity);

	//Set debug name
	SetDebugName(debugName);
}

void DirectionalLight::Render()
{
	if (!isActive)
		return;

	EngineAPI::Graphics::GraphicsDevice* device =
		EngineAPI::Graphics::GraphicsManager::GetInstance()->GetDevice();
	assert(device);

	//Write data to the shared dlight data cbuffer
	EngineAPI::Graphics::ConstantBuffer& cb = EngineAPI::Statics::GraphicsStatics::LightPass_DirectionalLight_LightDataCB;

	MappedResourceData mr = {};
	if (cb.MapResource(device, 0, RESOURCE_MAP_WRITE_DISCARD, &mr))
		memcpy(mr.MappedData, &lightData, sizeof(DirectionalLightGraphicsData));
	cb.UnmapResource(device, 0);

	//Render the light source during the lighting pass
	//
	//Bind the light data CBuffer (Slot 1)
	device->PSSetConstantBuffer(&cb, GRAPHICS_CONFIG_LIGHT_PASS_LIGHT_DATA_CBUFFER_BINDING_SLOT);

	//Draw
	device->Draw(4, 0);
}