#include "D3D11VertexShader.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11VertexShader::Shutdown()
{
	//release the input layout
	inputLayout.Shutdown();

	//Release shader
	ReleaseCOM(vertexShader);

	//Cleanup super
	__super::Shutdown();
}

bool D3D11VertexShader::InitCompiledVertexShaderFromFile(EngineAPI::Graphics::GraphicsDevice* device,
	const char* compiledShaderFile,
	VertexInputSignatureElementDescription* inputSignature, uint32_t inputsCount,
	std::string debugName)
{
	//Destroy old shader
	if (vertexShader)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old Vertex Shader: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(vertexShader);
	}

	//Store debug name
	SetDebugName(debugName);

	//Load shader byte code from file
	if (!ReadCompiledShaderFile(compiledShaderFile))
	{
		//File read failed - cleanup and print error to console
		CleanupBytecodeBuffer();

		std::string o = std::string(__FUNCTION__) + " Error: Could not read file: " + compiledShaderFile + ". Shader Debug Name: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
		return false;
	}

	const char* shaderByteCode = GetShaderBytecodeBuffer();
	SIZE_T byteCodeLength = GetShaderBytecodeBufferSize();

	//Class linkage - TODO??
	ID3D11ClassLinkage* linkage = nullptr;

	//Print message saying we are creating a vertex shader
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Vertex Shader: " + GetDebugName();
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create shader
	HR(device->GetD3D11Device()->CreateVertexShader(shaderByteCode, byteCodeLength,
		linkage, &vertexShader));

	//Debug name
	SetD3D11ResourceDebugName(vertexShader);

	//Generate the input signature.
	if (!inputLayout.InitVertexInputSignature(device, inputSignature, inputsCount, shaderByteCode, byteCodeLength, GetDebugName()))
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Failed to init the input layout/signature: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
		return false;
	}

	//Finished with bytecode data
	CleanupBytecodeBuffer();

	//Done
	return true;
}

bool D3D11VertexShader::BindVertexShaderToPipeline(EngineAPI::Graphics::GraphicsDevice* device)
{
	//
	//TODO: Shader interfaces (ID3D11ClassInstance**)
	//

	//Set input layout
	device->GetD3D11ImmediateContext()->IASetInputLayout(inputLayout.GetD3D11InputLayout());

	//Bind vs
	device->GetD3D11ImmediateContext()->VSSetShader(vertexShader, nullptr, 0);

	//Done
	return true;
}