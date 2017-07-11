#include "D3D11VertexInputSignature.h"

using namespace EngineAPI::Graphics::Platform;

bool D3D11VertexInputSignature::InitVertexInputSignature(EngineAPI::Graphics::GraphicsDevice* device, 
	VertexInputSignatureElementDescription* inputs, uint32_t inputsCount, 
	const char* vertexShaderBytecodeWithSignature, SIZE_T shaderByteCodeSize, 
	std::string ownerDebugName)
{
	assert(inputs != nullptr);
	assert(inputsCount > 0);
	assert(vertexShaderBytecodeWithSignature != nullptr);
	assert(shaderByteCodeSize > 0);

	//Recreate the input layout?
	if (inputLayout)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old input layout: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(inputLayout);
	}

	//Set debug name
	SetDebugName(ownerDebugName + "_VertexInputSignature");

	//For each input, generate a D3D11_INPUT_LAYOUT_DESC
	inputElementsArray.resize(inputsCount);
	for (int i = 0; i < inputsCount; ++i)
	{
		inputElementsArray[i].SemanticName = inputs[i].SemanticName.c_str();
		inputElementsArray[i].SemanticIndex = inputs[i].SemanticIndex;
		inputElementsArray[i].Format = (DXGI_FORMAT)inputs[i].InputFormat; //DXGI_FORMAT* and my RESOURCE_FORMAT_* have same int values so a cast is fine for now
		inputElementsArray[i].InputSlot = inputs[i].BufferInputSlotIndex;
		inputElementsArray[i].AlignedByteOffset = inputs[i].AlignedByteOffset;
		inputElementsArray[i].InputSlotClass = (D3D11_INPUT_CLASSIFICATION)inputs[i].InputType;
		inputElementsArray[i].InstanceDataStepRate = inputs[i].InstanceDataStepRate;
	}

	//Create the input layout (D3D11)
	HR(device->GetD3D11Device()->CreateInputLayout(inputElementsArray.data(), inputsCount, 
		vertexShaderBytecodeWithSignature, shaderByteCodeSize, &inputLayout));

	//Set resource debug name
	SetD3D11ResourceDebugName(inputLayout);

	//Done
	return true;
}

void D3D11VertexInputSignature::Shutdown()
{
	inputElementsArray.clear();
	ReleaseCOM(inputLayout);
}