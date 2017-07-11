#include "D3D11VertexShader.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11VertexShader::InitCompiledVertexShaderFromFile(EngineAPI::Graphics::GraphicsDevice* device,
	const char* compiledShaderFile,
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
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		return false;
	}

	const void* shaderByteCode = GetShaderBytecodeBuffer();
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

	//
	//Done with the VS bytecode (We could extract some info such as inputs here???)
	//
	CleanupBytecodeBuffer();

	//Done
	return true;
}

void D3D11VertexShader::Shutdown()
{
	//Release shader
	ReleaseCOM(vertexShader);

	//Cleanup super
	__super::Shutdown();
}