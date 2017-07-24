#include "D3D11GeometryShader.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11GeometryShader::Shutdown()
{
	//If still around...
	CleanupBytecodeBuffer();

	//Delete HS
	ReleaseCOM(geometryShader);

	//Cleanup super
	__super::Shutdown();
}

bool D3D11GeometryShader::InitCompiledGeometryShaderFromFile(EngineAPI::Graphics::GraphicsDevice* device,
	const char* compiledShaderFile,
	std::string debugName)
{
	//Destroy old shader
	if (geometryShader)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old Geometry Shader: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(geometryShader);
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
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Geometry Shader: " + debugName;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create shader
	HR_CHECK_ERROR(device->GetD3D11Device()->CreateGeometryShader(shaderByteCode, byteCodeLength, linkage, &geometryShader));
	if (geometryShader == nullptr)
		return false;

	//Cache a reference to the ID3D11DeviceChild pointer - BEFORE setting the D3D11
	//resource debug name
	CacheD3D11DeviceChildInterface(geometryShader);

	//Debug name
	SetDebugName(debugName);

	//
	//Done with the GS bytecode (We could extract some info such as inputs here???)
	//
	CleanupBytecodeBuffer();

	//Done
	return true;
}

bool D3D11GeometryShader::BindHullShaderToPipeline(EngineAPI::Graphics::GraphicsDevice* device)
{
	//
	//TODO: Shader interfaces (ID3D11ClassInstance**)
	//

	//Bind
	device->GetD3D11ImmediateContext()->GSSetShader(geometryShader, nullptr, 0);

	//Done
	return true;
}
