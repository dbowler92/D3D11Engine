#include "D3D11IndexBuffer.h"

//statics
#include "../../Statics/D3D11Statics/D3D11Statics.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11IndexBuffer::Shutdown()
{
	//Cleanup buffer && everything in the inheritance chain
	__super::Shutdown();
}

bool D3D11IndexBuffer::InitIndexBuffer(EngineAPI::Graphics::GraphicsDevice* device,
	IndexBufferFormat indexBufferFormat, uint32_t indexCount,
	void* indexData,
	ResourceUsage usage,
	ResourceCPUAccessFlag cpuAccess,
	ResourceBindFlag resourceBinding,
	std::string debugName)
{
	//Some error checking
	if (indexData == nullptr && usage == RESOURCE_USAGE_IMMUTABLE)
	{
		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11IndexBuffer::InitIndexBuffer(): Immutable buffer created without any initial data");
		return false;
	}
	if (indexData == nullptr && usage == RESOURCE_USAGE_DEFAULT)
	{
		EngineAPI::Debug::DebugLog::PrintErrorMessage("VERIFY THIS ERROR: D3D11IndexBuffer::InitIndexBuffer(): Default (GPU access only) buffer created without any initial data");
		return false;
	}
	if ((resourceBinding & RESOURCE_BIND_INDEX_BUFFER_BIT) == false)
	{
		//Append index buffer usage
		resourceBinding |= RESOURCE_BIND_INDEX_BUFFER_BIT;

		//EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11IndexBuffer::InitIndexBuffer(): Index buffer created without the index buffer bind flag.");
		//return false;
	}

	//Store index count and format
	this->indexCount = indexCount;
	this->indexBufferFormat = indexBufferFormat;

	//Calculate the usage flag and cpu access flag in D3D11 world
	D3D11_USAGE d3d11Usage = D3D11_USAGE_DEFAULT;
	UINT d3d11CpuAccess = NULL;
	assert(EngineAPI::Statics::D3D11ResourceStatics::CalculateD3D11UsageFlags(usage, cpuAccess,
		d3d11Usage, d3d11CpuAccess));
	
	//Buffer size
	UINT bytesPerElement = (indexBufferFormat == INDEX_BUFFER_FORMAT_UINT16) ? 2 : 4;
	UINT bufferSizeBytes = indexCount * bytesPerElement;

	//Fill buffer desc. 
	bufferDesc.ByteWidth = bufferSizeBytes;
	bufferDesc.StructureByteStride = 0; //TODO
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = d3d11Usage;
	bufferDesc.CPUAccessFlags = d3d11CpuAccess;
	bufferDesc.BindFlags = resourceBinding; //Cast

	//Initial data structure
	bufferInitialData = {};
	bufferInitialData.pSysMem = indexData;
	bufferInitialData.SysMemPitch = 0;
	bufferInitialData.SysMemSlicePitch = 0;

	//Should use initial data?
	bool doesInitWithInitialData = (indexData != nullptr) ? true : false;

	//Init the buffer
	if (!InitBuffer(device, doesInitWithInitialData, debugName))
		return false;

	//Done
	return true;
}

void D3D11IndexBuffer::BindIndexBufferToPipeline(EngineAPI::Graphics::GraphicsDevice* device,
	UINT offset)
{
	device->GetD3D11ImmediateContext()->IASetIndexBuffer(buffer, (DXGI_FORMAT)indexBufferFormat, offset);
}