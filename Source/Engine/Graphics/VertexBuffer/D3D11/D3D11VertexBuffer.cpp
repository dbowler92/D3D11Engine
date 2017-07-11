#include "D3D11VertexBuffer.h"

//statics
#include "../../Statics/D3D11Statics/D3D11Statics.h"

using namespace EngineAPI::Graphics::Platform;

void D3D11VertexBuffer::Shutdown()
{
	//Cleanup buffer
	__super::Shutdown();
}

bool D3D11VertexBuffer::InitVertexBuffer(EngineAPI::Graphics::GraphicsDevice* device,
	uint32_t bufferSizeBytes, void* initialData,
	ResourceUsage usage, ResourceCPUAccessFlag cpuAccess, ResourceBindFlag resourceBinding,
	std::string debugName)
{
	//Some error checking
	if (initialData == nullptr && usage == RESOURCE_USAGE_IMMUTABLE)
	{
		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11VertexBuffer::InitVertexBuffer(): Immutable buffer created without any initial data");
		return false;
	}
	if (initialData == nullptr && usage == RESOURCE_USAGE_DEFAULT)
	{
		//
		//This does seem a little odd... TODO: Verify that default buffers do
		//need initial data -> My view is that this kind of buffer could be
		//used as, say, a stream out buffer and have its data allocated like so
		//
		//EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11VertexBuffer::InitVertexBuffer(): Default (GPU access only) buffer created without any initial data");
		//return false;
	}
	if ( (resourceBinding & RESOURCE_BIND_VERTEX_BUFFER_BIT) == false )
	{
		EngineAPI::Debug::DebugLog::PrintErrorMessage("D3D11VertexBuffer::InitVertexBuffer(): Vertex buffer created without the vertex buffer bind flag.");
		return false;
	}
	
	//Calculate the usage flag and cpu access flag in D3D11 world
	D3D11_USAGE d3d11Usage = D3D11_USAGE_DEFAULT;
	UINT d3d11CpuAccess = NULL;
	assert(EngineAPI::Statics::D3D11ResourceStatics::CalculateD3D11UsageFlags(usage, cpuAccess,
		d3d11Usage, d3d11CpuAccess));

	//Fill buffer desc. 
	bufferDesc.ByteWidth = bufferSizeBytes;
	bufferDesc.StructureByteStride = 0; //TODO
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = d3d11Usage;
	bufferDesc.CPUAccessFlags = d3d11CpuAccess;
	bufferDesc.BindFlags = resourceBinding; //Cast

	//Initial data structure
	D3D11_SUBRESOURCE_DATA initialDataDesc = {};
	initialDataDesc.pSysMem = initialData;
	
	//Init the buffer
	if (!InitBuffer(device, &initialDataDesc, debugName))
		return false;

	//Done
	return true;
}

void D3D11VertexBuffer::BindVertexBufferToPipeline(EngineAPI::Graphics::GraphicsDevice* device, 
	UINT stride, UINT offset)
{
	device->GetD3D11ImmediateContext()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}