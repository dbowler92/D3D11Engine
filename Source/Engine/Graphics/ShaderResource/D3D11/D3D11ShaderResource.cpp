#include "D3D11ShaderResource.h"

//File IO for now
#include <fstream>

using namespace EngineAPI::Graphics::Platform;

void D3D11ShaderResource::Shutdown() 
{
	//If we still have shader byte code buffer, delete it now
	CleanupBytecodeBuffer();

	//No longer need the reference to the baseShaderInterface
	//ReleaseCOM(baseShaderInterface);
	baseShaderInterface = nullptr;

	//Shutdown core object
	__super::Shutdown();
}

void D3D11ShaderResource::SetDebugName(std::string s)
{
	//CoreObject debug name
	__super::SetDebugName(s);

	//D3D11 Resource
	if (baseShaderInterface)
		baseShaderInterface->SetPrivateData(WKPDID_D3DDebugObjectName, s.size(), s.c_str());
}

bool D3D11ShaderResource::ReadCompiledShaderFile(const char* shaderFile)
{
	//Print message
	std::string o = std::string(__FUNCTION__) + ": Reading file: " + shaderFile;
	EngineAPI::Debug::DebugLog::PrintInfoMessage(o.c_str());

	//
	//TODO: File IO
	//
	//Open file
	std::ifstream file(shaderFile, std::ios::ate | std::ios::binary); //Start read at the end -> We can then easily calculate the file size
	if (!file.is_open())
	{
		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11ShaderResource::ReadCompiledShaderFile() Error: Could not open file\n");
		return false;
	}

	//Is open, copy data. Alloc memory for our buffer && cache size
	shaderBytecodeSize = (size_t)file.tellg();
	shaderBytecodeBuffer = new char[shaderBytecodeSize];
	assert(shaderBytecodeBuffer != nullptr);

	//Seek to beginning of the file before copy
	file.seekg(0);

	//Copy data
	file.read(shaderBytecodeBuffer, shaderBytecodeSize);

	//Close file and return
	file.close();

	//Done
	return true;
}

void D3D11ShaderResource::CleanupBytecodeBuffer()
{
	if (shaderBytecodeBuffer != nullptr)
	{
		delete[] shaderBytecodeBuffer;
		shaderBytecodeBuffer = nullptr;
		shaderBytecodeSize = 0;
	}
}

