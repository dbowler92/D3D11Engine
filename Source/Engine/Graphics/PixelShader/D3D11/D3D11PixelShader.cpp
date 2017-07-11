#include "D3D11PixelShader.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11PixelShader::InitCompiledPixelShaderFromFile(EngineAPI::Graphics::GraphicsDevice* device,
	const char* compiledShaderFile,
	std::string debugName)
{
	//Destroy old shader
	if (pixelShader)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old Pixel Shader: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(pixelShader);
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

	//Print message saying we are creating a pixel shader
	std::string o = std::string(__FUNCTION__) + ": " + "Creating Pixel Shader: " + GetDebugName();
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//Create shader
	HR(device->GetD3D11Device()->CreatePixelShader(shaderByteCode, byteCodeLength,
		linkage, &pixelShader));

	//Debug name
	SetD3D11ResourceDebugName(pixelShader);

	//
	//Done with the PS bytecode (We could extract some info such as inputs here???)
	//
	CleanupBytecodeBuffer();

	//Done
	return true;
}

void D3D11PixelShader::Shutdown()
{
	//Delete PS
	ReleaseCOM(pixelShader);

	//Cleanup super
	__super::Shutdown();
}