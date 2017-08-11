#include "SpotLight.h"

//Graphics manager
#include <Graphics/GraphicsManager/GraphicsManager.h>

//Uses some of the static engine resources
#include <Statics/GraphicsStatics/GraphicsStatics.h>

//Scene camera
#include <Rendering/VirtualCamera/VirtualCamera.h>

using namespace EngineAPI::Rendering;

//local space direction of the cone generated in the DS + the 
//world forward vector. null rotational transform on this
//light == this vector for the direction!
const XMFLOAT4 CONE_FORWARD_VECTOR4 = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

void SpotLight::Shutdown()
{
	//Shutdown self
	isActive = false;

	//Shutdown super
	__super::Shutdown();
}

void SpotLight::InitSpotLightSource(XMFLOAT3 position, float range,
	XMFLOAT3 rotationTransformRollPitchYawDegrees,
	float lightOuterAngleDeg, float lightInnerAngleDeg,
	XMFLOAT3 col, float intensity,
	std::string debugName)
{
	//Cache rotation
	lightRotationRollPitchYawDegrees = rotationTransformRollPitchYawDegrees;

	//Calculate direction from rotation
	XMVECTOR dir = XMLoadFloat4(&CONE_FORWARD_VECTOR4);
	XMMATRIX dirRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(lightRotationRollPitchYawDegrees.x),
		XMConvertToRadians(lightRotationRollPitchYawDegrees.y),
		XMConvertToRadians(lightRotationRollPitchYawDegrees.z));
	dir = XMVector4Transform(dir, dirRot);

	//Normalize direction
	dir = XMVector3Normalize(dir);
	XMFLOAT3 dNormalized;
	XMStoreFloat3(&dNormalized, dir);

	//Cache data
	lightData.LightPosition = position;
	lightData.LightRange = range;
	lightData.LightColourAndIntensity = XMFLOAT4(col.x, col.y, col.z, intensity);
	lightData.LightDirection = dNormalized;
	lightData.LightOuterAngle = lightOuterAngleDeg;
	lightData.LightInnerAngle = lightInnerAngleDeg;
	
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

	//Light world matrix - Note: For rotation, the local space cone we 
	//generate in the domain shader is pointing in the +Z axis
	XMVECTOR coneForward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	XMVECTOR lightDir = XMLoadFloat3(&lightData.LightDirection);
	XMVECTOR coneRight = XMVector3Cross(coneForward, lightDir);

	XMMATRIX lightRotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(lightRotationRollPitchYawDegrees.x),
		XMConvertToRadians(lightRotationRollPitchYawDegrees.y),
		XMConvertToRadians(lightRotationRollPitchYawDegrees.z));
	XMMATRIX lightTranslation = XMMatrixTranslation(lightData.LightPosition.x, lightData.LightPosition.y, lightData.LightPosition.z);
	XMMATRIX lightWorld = lightRotation * lightTranslation;

	//Calculate light world * view * projection matrix
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