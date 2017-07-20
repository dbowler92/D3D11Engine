#include "TestCube.h"

//Graphics manager
#include <Graphics/GraphicsManager/GraphicsManager.h>

struct VertexPosTexNormal
{
	VertexPosTexNormal() {};
	VertexPosTexNormal(float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty)
	{
		p[0] = px;
		p[1] = py;
		p[2] = pz;

		n[0] = nx;
		n[1] = ny;
		n[2] = nz;

		t[0] = tx;
		t[1] = ty;
	}

	float p[3];
	float n[3];
	float t[2];
};

const UINT CUBE_NON_INDEXED_VERTS_COUNT = 36;
const VertexPosTexNormal CUBE_TEX_VERTS[36] =
{
	{ -1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
	{ -1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
	{ -1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f },

	{ -1.0f,-1.0f,-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f },
	{ 1.0f,-1.0f,-1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f },
	{ -1.0f,-1.0f,-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f },
	{ -1.0f, 1.0f,-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f,-1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f },

	{ -1.0f,-1.0f,-1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f,-1.0f,-1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f },
	{ -1.0f,-1.0f,-1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f },
	{ -1.0f,-1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f },

	{ -1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f },

	{ 1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
	{ 1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },

	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
	{ -1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
};

void TestCube::Shutdown()
{
	texCubeVB.Shutdown();
	texVS.Shutdown();
	texPS.Shutdown();
	textureFromFile.Shutdown();
	texSRV.Shutdown();
	defaultLinearSampler.Shutdown();
	bs.Shutdown();
	dss.Shutdown();
	rss.Shutdown();

	 vsCBPerObject.Shutdown();
	 psCBPerObject.Shutdown();

	__super::Shutdown();
}

void TestCube::InitCube(EngineAPI::Graphics::GraphicsDevice* device,
	CXMMATRIX& cubeWorld,
	std::string diffuseTex, bool isPNG,
	float specPower, float specIntensity,
	std::string debugName)
{
	__super::SetDebugName(debugName);

	assert(isPNG == true);

	//World matrix init
	world = cubeWorld;

	//VB
	assert(texCubeVB.InitVertexBuffer(device,
		sizeof(VertexPosTexNormal), CUBE_NON_INDEXED_VERTS_COUNT, (void*)CUBE_TEX_VERTS,
		RESOURCE_USAGE_IMMUTABLE, NULL, RESOURCE_BIND_VERTEX_BUFFER_BIT,
		std::string((debugName + "_VB"))));

	//Shaders
	VertexInputSignatureElementDescription vbDesc[3];
	vbDesc[0] = VertexInputSignatureElementDescription::PerVertex("POSITION", 0, RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0);
	vbDesc[1] = VertexInputSignatureElementDescription::PerVertex("NORMAL", 0,  RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 12);
	vbDesc[2] = VertexInputSignatureElementDescription::PerVertex("TEXCOORD", 0, RESOURCE_FORMAT_R32G32_FLOAT, 0, 24);

	assert(texVS.InitCompiledVertexShaderFromFile(device,
		SHADER_COMPILED_ASSETS_FOLDER"G_TexturedVS.cso",
		&vbDesc[0], 3,
		std::string(debugName + "_VS")));

	assert(texPS.InitCompiledPixelShaderFromFile(device,
		SHADER_COMPILED_ASSETS_FOLDER"G_TexturedPS.cso",
		std::string(debugName + "_PS")));

	//Texture
	if (isPNG)
	{
		assert(textureFromFile.InitTexture2DFromPNGFile(device,
			diffuseTex, true,
			std::string(debugName + "_Tex")));
	}
	
	assert(texSRV.InitShaderResourceViewToTexture2D(device,
		&textureFromFile, true, RESOURCE_FORMAT_R8G8B8A8_UNORM, false,
		std::string(debugName + "_SRV")));

	SamplerStateDescription linearSamplerDesc = {};
	assert(defaultLinearSampler.InitSamplerState(device, &linearSamplerDesc,
		std::string(debugName + "_LinSampler")));

	if (textureFromFile.DoesSupportAutoMipmapsGeneration()) 	//Generate mips?
		assert(textureFromFile.AutoGenerateMipmaps(device, &texSRV));

	//Pipeline states
	DepthStencilPipelineStateDescription dssState = {};
	assert(dss.InitDepthStencilState(device, &dssState, std::string("TestDSS")));

	RasterizerPipelineStateDescription rssState = {};
	assert(rss.InitRasterizerState(device, &rssState, std::string("TestRSS")));

	BlendPipelineStateDescription bsDesc = {};
	assert(bs.InitBlendState(device, &bsDesc, std::string("TestBS")));

	//CBuffers
	XMFLOAT4X4 w;
	XMStoreFloat4x4(&w, world);
	assert(vsCBPerObject.InitConstantBuffer(device, sizeof(XMFLOAT4X4), (void*)&w, 
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT, RESOURCE_BIND_CONSTANT_BUFFER_BIT, 
		std::string("TestCube_VSCB")));

	float matData[4];
	matData[0] = specPower;
	matData[1] = specIntensity;
	matData[2] = 0.0f;
	matData[3] = 0.0f;
	assert(psCBPerObject.InitConstantBuffer(device, sizeof(float) * 4, (void*)&matData,                 //Min 16 bytes
		RESOURCE_USAGE_DYNAMIC, RESOURCE_CPU_ACCESS_WRITE_BIT, RESOURCE_BIND_CONSTANT_BUFFER_BIT, //Leave DEFAULT for now...
		std::string("TestCube_PSCB")));
}

void TestCube::Update(float dt)
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

void TestCube::RenderToGBuffer(EngineAPI::Graphics::GraphicsDevice* device)
{
	//Topology
	device->IASetTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Shaders
	device->VSSetShader(&texVS);
	device->PSSetShader(&texPS);

	//Vertex (and index) buffer
	device->IASetVertexBuffer(&texCubeVB, 0);

	//CBuffers
	device->VSSetConstantBuffer(&vsCBPerObject, 1);
	device->PSSetConstantBuffer(&psCBPerObject, 2);

	//Shader resource
	device->PSSetShaderResource(&texSRV, 0);

	//Rendering state
	device->RSSetState(&rss);
	device->OMSetBlendState(&bs);
	device->OMSetDepthStencilState(&dss, 0);

	//Sampler state object
	device->PSSetSamplerState(&defaultLinearSampler, 0);

	//Draw command
	device->Draw(texCubeVB.GetElementsCount(), 0);
}