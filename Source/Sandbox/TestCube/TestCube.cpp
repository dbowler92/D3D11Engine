#include "TestCube.h"

struct TexVertex
{
	TexVertex() {};
	TexVertex(float px, float py, float pz, float tx, float ty)
	{
		p[0] = px;
		p[1] = py;
		p[2] = pz;

		t[0] = tx;
		t[1] = ty;
	}

	float p[3];
	float t[2];
};

const UINT CUBE_NON_INDEXED_VERTS_COUNT = 36;
const TexVertex CUBE_TEX_VERTS[36] =
{
	{ -1.0f,-1.0f,-1.0f, 1.0f, 1.0f },
	{ -1.0f,-1.0f, 1.0f, 0.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f,-1.0f, 1.0f, 0.0f },
	{ -1.0f,-1.0f,-1.0f, 1.0f, 1.0f },

	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f,  1.0f, 0.0f },
	{ 1.0f,-1.0f,-1.0f,  1.0f, 1.0f },
	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f },
	{ -1.0f, 1.0f,-1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f,-1.0f,  1.0f, 0.0f },

	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f },
	{ 1.0f,-1.0f,-1.0f,  1.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f,  1.0f, 0.0f },
	{ -1.0f,-1.0f,-1.0f, 0.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f,  1.0f, 0.0f },
	{ -1.0f,-1.0f, 1.0f, 0.0f, 0.0f },

	{ -1.0f, 1.0f,-1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
	{ -1.0f, 1.0f,-1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f,  1.0f, 0.0f },

	{ 1.0f, 1.0f,-1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
	{ 1.0f,-1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f,-1.0f,-1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f,-1.0f, 0.0f, 0.0f },

	{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ -1.0f,-1.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
	{ -1.0f,-1.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f,-1.0f, 1.0f,  1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f,  1.0f, 0.0f },
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

	__super::Shutdown();
}

void TestCube::InitCube(EngineAPI::Graphics::GraphicsDevice* device,
	CXMMATRIX& cubeWorld,
	std::string diffuseTex, bool isPNG,
	std::string debugName)
{
	__super::SetDebugName(debugName);

	assert(isPNG == true);

	//World matrix init
	world = XMMatrixIdentity();
	world = cubeWorld;

	//VB
	assert(texCubeVB.InitVertexBuffer(device,
		sizeof(TexVertex), CUBE_NON_INDEXED_VERTS_COUNT, (void*)CUBE_TEX_VERTS,
		RESOURCE_USAGE_IMMUTABLE, NULL, RESOURCE_BIND_VERTEX_BUFFER_BIT,
		std::string((debugName + "_VB"))));

	//Shaders
	VertexInputSignatureElementDescription vbDesc[2];
	vbDesc[0] = VertexInputSignatureElementDescription::PerVertex("POSITION", 0, RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0);
	vbDesc[1] = VertexInputSignatureElementDescription::PerVertex("TEXCOORD", 0, RESOURCE_FORMAT_R32G32_FLOAT, 0, 12);

	assert(texVS.InitCompiledVertexShaderFromFile(device,
		SHADER_COMPILED_ASSETS_FOLDER"TestTexturedVS.cso",
		&vbDesc[0], 2,
		std::string(debugName + "_VS")));

	assert(texPS.InitCompiledPixelShaderFromFile(device,
		SHADER_COMPILED_ASSETS_FOLDER"TestTexturedPS.cso",
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
}

void TestCube::Update(float dt)
{}

void TestCube::Render(EngineAPI::Graphics::GraphicsDevice* device)
{
	//Topology
	device->IASetTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Shaders
	device->VSSetShader(&texVS);
	device->PSSetShader(&texPS);

	//Vertex (and index) buffer
	device->IASetVertexBuffer(&texCubeVB, 0);

	//Shader resource / cbuffers
	//device->VSSetConstantBuffer(&constantBuffer, 0);
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

void TestCube::RenderToGBuffer(EngineAPI::Graphics::GraphicsDevice* device)
{
	//Topology
	device->IASetTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Shaders
	device->VSSetShader(&texVS);
	device->PSSetShader(&texPS);

	//Vertex (and index) buffer
	device->IASetVertexBuffer(&texCubeVB, 0);

	//Shader resource / cbuffers
	//device->VSSetConstantBuffer(&constantBuffer, 0);
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