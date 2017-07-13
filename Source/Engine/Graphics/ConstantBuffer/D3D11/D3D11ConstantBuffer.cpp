#include "D3D11ConstantBuffer.h"

//Statics 
#include "../../Statics/D3D11Statics/D3D11Statics.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11ConstantBuffer::Shutdown()
{
	//Cleanup buffer && everything in the inheritance chain
	__super::Shutdown();
}

bool D3D11ConstantBuffer::InitConstantBuffer(EngineAPI::Graphics::GraphicsDevice* device,
	uint32_t constantBufferSizeBytes, void* initialData,
	ResourceUsage usage,
	ResourceCPUAccessFlag cpuAccess,
	ResourceBindFlag resourceBinding,
	std::string debugName)
{
	//Error checking
	if (initialData == nullptr && usage == RESOURCE_USAGE_IMMUTABLE)
	{
		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11ConstantBuffer::InitConstantBuffer(): Immutable buffer created without any initial data");
		return false;
	}
	if (initialData == nullptr && usage == RESOURCE_USAGE_DEFAULT)
	{
		//
		//This does seem a little odd... TODO: Verify that default buffers do
		//need initial data -> My view is that this kind of buffer could be
		//used as, say, a stream out buffer and have its data allocated like so
		//
		EngineAPI::Debug::DebugLog::PrintErrorMessage("VERIFY THIS ERROR: D3D11ConstantBuffer::InitConstantBuffer(): Default (GPU access only) buffer created without any initial data");
		return false;
	}
	if ((resourceBinding & RESOURCE_BIND_CONSTANT_BUFFER_BIT) == false)
	{
		//Append constant buffer usage
		resourceBinding |= RESOURCE_BIND_CONSTANT_BUFFER_BIT;

		//EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11ConstantBuffer::InitConstantBuffer(): Constant buffer created without the constant buffer bind flag.");
		//return false;
	}

	//Cache the buffer size
	this->cBufferSizeBytes = constantBufferSizeBytes;

	//Calculate the usage flag and cpu access flag in D3D11 world
	D3D11_USAGE d3d11Usage = D3D11_USAGE_DEFAULT;
	UINT d3d11CpuAccess = NULL;
	assert(EngineAPI::Statics::D3D11ResourceStatics::CalculateD3D11UsageFlags(usage, cpuAccess,
		d3d11Usage, d3d11CpuAccess));

	//Fill desc description structure
	bufferDesc.ByteWidth = constantBufferSizeBytes;
	bufferDesc.StructureByteStride = 0; //TODO
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = d3d11Usage;
	bufferDesc.CPUAccessFlags = d3d11CpuAccess;
	bufferDesc.BindFlags = resourceBinding; //Cast

	//Initial data structure
	bufferInitialData = {};
	bufferInitialData.pSysMem = initialData;
	bufferInitialData.SysMemPitch = 0;
	bufferInitialData.SysMemSlicePitch = 0;

	//Should use initial data?
	bool doesInitWithInitialData = (initialData != nullptr) ? true : false;

	//Init the buffer
	if (!InitBuffer(device, doesInitWithInitialData, usage, cpuAccess, resourceBinding, debugName))
		return false;

	//Done
	return true;
}

void D3D11ConstantBuffer::BindConstantBufferToVertexShaderStage(EngineAPI::Graphics::GraphicsDevice* device, UINT bufferSlot)
{
	device->GetD3D11ImmediateContext()->VSSetConstantBuffers(bufferSlot, 1, &buffer);
}