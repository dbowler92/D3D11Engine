//D3D11Statics.h
//Created 11/07/17
//Created By Daniel Bowler
//
//Static helper functions for D3D11

#pragma once

//Graphics
#include "../../Includes/GraphicsIncludes.h"

namespace EngineAPI
{
	namespace Statics
	{
		class D3D11ResourceStatics
		{
			MAKE_STATICS_CLASS_ONLY(D3D11ResourceStatics);
		public:
			//Calculates the D3D11 usage flag && CPU access flag
			//resources based on the engine usage && cpu access flag passed
			//to each resource at init time. 
			//
			//Returns false on error
			static bool CalculateD3D11UsageFlags(ResourceUsage usage, ResourceCPUAccessFlag cpuAccess, 
				D3D11_USAGE& d3d11UsageOut, UINT& d3d11CPUAccessFlagOut);

			//Sets the debug name of an ID3D11Resource
			static void SetD3D11ResourceDebugName(ID3D11DeviceChild* child, std::string name);
		};
	};
};
