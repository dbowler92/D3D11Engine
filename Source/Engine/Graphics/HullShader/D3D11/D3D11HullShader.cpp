#include "D3D11HullShader.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11HullShader::Shutdown()
{
	//If still around...
	CleanupBytecodeBuffer();

	//Delete HS
	ReleaseCOM(hullShader);

	//Cleanup super
	__super::Shutdown();
}

bool D3D11HullShader::InitCompiledHullShaderFromFile(EngineAPI::Graphics::GraphicsDevice* device,
	const char* compiledShaderFile,
	std::string debugName)
{
	//Destroy old shader
	if (hullShader)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old Hull Shader: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(hullShader);
	}

	//Load shader byte code from file
	if (!ReadCompiledShaderFile(compiledShaderFile))
	{
		//File read failed - cleanup and print error to console
		CleanupBytecodeBuffer();

		std::string o = std::string(__FUNCTION__) + " Error: Could not read file: " + compiledShaderFile + ". Shader Debug Name: " + debugName;
		EngineAPI::Debug::DebugLog::PrintErrorMessage(o.c_str());
		return false;
	}

	const char* shaderByteCode = GetShaderBytecodeBuffer();
	SIZE_T byteCodeLength = GetShaderBytecodeBufferSize();

	//Class linkage - TODO??
	ID3D11ClassLinkage* linkage = nullptr;

	//Print message saying we are creating a pixel shader
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Hull Shader: " + debugName;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create shader
	HR_CHECK_ERROR(device->GetD3D11Device()->CreateHullShader(shaderByteCode, byteCodeLength, linkage, &hullShader));
	if (hullShader == nullptr)
		return false;

	//Cache a reference to the ID3D11DeviceChild pointer - BEFORE setting the D3D11
	//resource debug name
	CacheD3D11DeviceChildInterface(hullShader);

	//Debug name
	SetDebugName(debugName);

	//
	//Done with the HS bytecode (We could extract some info such as inputs here???)
	//
	CleanupBytecodeBuffer();

	//Done
	return true;
}

bool D3D11HullShader::BindHullShaderToPipeline(EngineAPI::Graphics::GraphicsDevice* device)
{
	//
	//TODO: Shader interfaces (ID3D11ClassInstance**)
	//

	//Bind
	device->GetD3D11ImmediateContext()->HSSetShader(hullShader, nullptr, 0);

	//Done
	return true;
}
