#include "D3D11VertexInputSignature.h"

//Statics (D3D11)
#include <Statics/D3D11Statics/D3D11Statics.h>

using namespace EngineAPI::Graphics::Platform;

bool D3D11VertexInputSignature::InitVertexInputSignature(EngineAPI::Graphics::GraphicsDevice* device, 
	VertexInputSignatureElementDescription* inputs, uint32_t inputsCount, 
	const char* vertexShaderBytecodeWithSignature, SIZE_T shaderByteCodeSize, 
	std::string ownerDebugName)
{
	//Recreate the input layout?
	if (inputLayout)
	{
		std::string o = std::string(__FUNCTION__) + ": " + "Releasing old input layout: " + GetDebugName();
		EngineAPI::Debug::DebugLog::PrintWarningMessage(o.c_str());
		ReleaseCOM(inputLayout);
	}

	if (inputsCount == 0)
	{
		SetDebugName(ownerDebugName + "_NULL_VertexInputSignature");

		//May not be an error - it is possible to bind 
		//a null VB and generate vertex data in the vertex
		//shader, for example
		EngineAPI::Debug::DebugLog::PrintWarningMessage("D3D11VertexInputSignature::InitVertexInputSignature() Warning: inputsCount == 0. No signature being created");
		return true;
	}

	assert(inputs != nullptr);
	assert(vertexShaderBytecodeWithSignature != nullptr);
	assert(shaderByteCodeSize > 0);

	//Set debug name
	SetDebugName(ownerDebugName + "_VertexInputSignature");

	//For each input, generate a D3D11_INPUT_ELEMENT_DESC
	if (inputsCount > 0)
	{
		inputElementsArray.resize(inputsCount);
		for (int i = 0; i < (int)inputsCount; ++i)
		{
			inputElementsArray[i].SemanticName = inputs[i].SemanticName.c_str();
			inputElementsArray[i].SemanticIndex = inputs[i].SemanticIndex;
			inputElementsArray[i].Format = (DXGI_FORMAT)inputs[i].InputFormat; //DXGI_FORMAT* and my RESOURCE_FORMAT_* have same int values so a cast is fine for now
			inputElementsArray[i].InputSlot = inputs[i].BufferInputSlotIndex;
			inputElementsArray[i].AlignedByteOffset = inputs[i].AlignedByteOffset;
			inputElementsArray[i].InputSlotClass = (D3D11_INPUT_CLASSIFICATION)inputs[i].InputType;
			inputElementsArray[i].InstanceDataStepRate = inputs[i].InstanceDataStepRate;
		}
	}

	//Create the input layout (D3D11)
	D3D11_INPUT_ELEMENT_DESC* inputLayoutDescPtr = nullptr;
	if (inputsCount > 0)
		inputLayoutDescPtr = inputElementsArray.data();

	HR_CHECK_ERROR(device->GetD3D11Device()->CreateInputLayout(inputLayoutDescPtr, inputsCount,
		vertexShaderBytecodeWithSignature, shaderByteCodeSize, &inputLayout));
	if (inputLayout == nullptr)
		return false;

	//Set resource debug name
	EngineAPI::Statics::D3D11ResourceStatics::SetD3D11ResourceDebugName(inputLayout, GetDebugName());

	//Done
	return true;
}

void D3D11VertexInputSignature::Shutdown()
{
	inputElementsArray.clear();
	ReleaseCOM(inputLayout);

	//Super
	__super::Shutdown();
}