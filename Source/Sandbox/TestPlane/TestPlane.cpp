#include "TestPlane.h"

//Geometry generator
#include <3rdParty/GeometryGenerator/GeometryGenerator.h>

//Graphics manager
#include <Graphics/GraphicsManager/GraphicsManager.h>

struct VertexPosNormalTangentTex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT2 tex;
};

void TestPlane::Shutdown()
{
	planeVB.Shutdown();
	planeIB.Shutdown();

	texNormalMappedVS.Shutdown();
	texNormalMappedPS.Shutdown();

	diffuseTex.Shutdown();
	diffuseTexSRV.Shutdown();

	normalTex.Shutdown();
	normalTexSRV.Shutdown();

	defaultLinearSampler.Shutdown();

	vsCBPerObject.Shutdown();
	psCBPerObject.Shutdown();

	__super::Shutdown();
}

void TestPlane::InitPlane(EngineAPI::Graphics::GraphicsDevice* device,
	CXMMATRIX& wd,
	float width, float depth,
	std::string diffuseTexFile, std::string normalTexFile,
	float specPower, float specIntensity,
	std::string debugName)
{
	__super::SetDebugName(debugName);

	//World matrix init
	world = wd;

	GeometryGenerator geoGen;
	GeometryGenerator::MeshData md;
	geoGen.CreateGrid(width, depth, 2, 2, md);
	
	std::vector<VertexPosNormalTangentTex> verts(md.Vertices.size());
	for (int i = 0; i < md.Vertices.size(); ++i)
	{
		verts[i].pos = md.Vertices[i].Position;
		verts[i].normal = md.Vertices[i].Normal;
		verts[i].tangent = md.Vertices[i].TangentU;
		verts[i].tex = md.Vertices[i].TexC;
	}

	std::vector<uint16_t>inds(md.Indices.size());
	for (int i = 0; i < md.Indices.size(); ++i)
		inds[i] = md.Indices[i];

	//VB
	assert(planeVB.InitVertexBuffer(device,
		sizeof(VertexPosNormalTangentTex), verts.size(), (void*)verts.data(),
		RESOURCE_USAGE_IMMUTABLE, NULL, RESOURCE_BIND_VERTEX_BUFFER_BIT,
		std::string((debugName + "_VB"))));

	//IB
	assert(planeIB.InitIndexBuffer(device,
		INDEX_BUFFER_FORMAT_UINT16, inds.size(), (void*)inds.data(),
		RESOURCE_USAGE_IMMUTABLE, NULL, RESOURCE_BIND_INDEX_BUFFER_BIT,
		std::string(debugName + "_IB")));

	//Shaders
	VertexInputSignatureElementDescription vbDesc[4];
	vbDesc[0] = VertexInputSignatureElementDescription::PerVertex("POSITION", 0, RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0);
	vbDesc[1] = VertexInputSignatureElementDescription::PerVertex("NORMAL", 0,  RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 12);
	vbDesc[2] = VertexInputSignatureElementDescription::PerVertex("TANGENT", 0, RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 24);
	vbDesc[3] = VertexInputSignatureElementDescription::PerVertex("TEXCOORD", 0, RESOURCE_FORMAT_R32G32_FLOAT, 0, 36);

	assert(texNormalMappedVS.InitCompiledVertexShaderFromFile(device,
		SHADER_COMPILED_ASSETS_FOLDER"G_NormalMappingVS.cso",
		&vbDesc[0], 4,
		std::string(debugName + "_VS")));

	assert(texNormalMappedPS.InitCompiledPixelShaderFromFile(device,
		SHADER_COMPILED_ASSETS_FOLDER"G_NormalMappingPS.cso",
		std::string(debugName + "_PS")));

	//Diffuse Texture
	assert(diffuseTex.InitTexture2DFromDDSFile(device,
		diffuseTexFile,
		std::string(debugName + "_Tex")));
	
	assert(diffuseTexSRV.InitShaderResourceViewToTexture2D(device,
		&diffuseTex, true, RESOURCE_FORMAT_R8G8B8A8_UNORM, false,
		std::string(debugName + "_SRV")));

	//Normal texture
	assert(normalTex.InitTexture2DFromDDSFile(device,
		normalTexFile,
		std::string(debugName + "_NormalTex")));

	assert(normalTexSRV.InitShaderResourceViewToTexture2D(device,
		&normalTex, true, RESOURCE_FORMAT_R8G8B8A8_UNORM, false,
		std::string(debugName + "_NormalSRV")));

	//Sampler state
	SamplerStateDescription linearSamplerDesc = {};
	assert(defaultLinearSampler.InitSamplerState(device, &linearSamplerDesc,
		std::string(debugName + "_LinSampler")));

	//if (diffuseTex.DoesSupportAutoMipmapsGeneration()) 	//Generate mips?
	//	assert(diffuseTex.AutoGenerateMipmaps(device, &diffuseTexSRV));

	//CBuffers
	XMFLOAT4X4 w;
	XMStoreFloat4x4(&w, world);
	assert(vsCBPerObject.InitConstantBuffer(device, sizeof(XMFLOAT4X4), (void*)&w, 
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT, RESOURCE_BIND_CONSTANT_BUFFER_BIT, 
		std::string("TestPlane_VSCB")));

	float matData[4];
	matData[0] = specPower;
	matData[1] = specIntensity;
	matData[2] = 0.0f;
	matData[3] = 0.0f;
	assert(psCBPerObject.InitConstantBuffer(device, sizeof(float) * 4, (void*)&matData,                 //Min 16 bytes
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT, RESOURCE_BIND_CONSTANT_BUFFER_BIT, //Leave DEFAULT for now...
		std::string("TestPlane_PSCB")));
}

void TestPlane::Update(float dt)
{
	EngineAPI::Graphics::GraphicsDevice* device = 
		EngineAPI::Graphics::GraphicsManager::GetInstance()->GetDevice();

	//Update CBuffers
	XMMATRIX wT = XMMatrixTranspose(world);
	XMFLOAT4X4 w;
	XMStoreFloat4x4(&w, wT);
 
	MappedResourceData mrd;
	bool didMap = vsCBPerObject.MapResource(device, 0, RESOURCE_MAP_WRITE_DISCARD, &mrd);
	if (didMap)
		memcpy(mrd.MappedData, (void*)&w, vsCBPerObject.GetConstantBufferSizeBytes());
	vsCBPerObject.UnmapResource(device, 0);
}

void TestPlane::RenderToGBuffer(EngineAPI::Graphics::GraphicsDevice* device)
{
	//Topology
	device->IASetTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Shaders
	device->VSSetShader(&texNormalMappedVS);
	device->PSSetShader(&texNormalMappedPS);

	//Vertex (and index) buffer
	device->IASetVertexBuffer(&planeVB, 0, 0);
	device->IASetIndexBuffer(&planeIB, 0);

	//CBuffers
	device->VSSetConstantBuffer(&vsCBPerObject, 1);
	device->PSSetConstantBuffer(&psCBPerObject, 2);

	//Shader resources
	device->PSSetShaderResource(&diffuseTexSRV, 0);
	device->PSSetShaderResource(&normalTexSRV, 1);


	//Rendering state
	//

	//Sampler state object
	device->PSSetSamplerState(&defaultLinearSampler, 0);

	//Draw command
	//device->Draw(planeVB.GetElementsCount(), 0);
	device->DrawIndexed(planeIB.GetIndexCount(), 0, 0);
}