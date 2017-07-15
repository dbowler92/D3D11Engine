//D3D11TextureFile2D.h
//Created 15/07/17
//Created By Daniel Bowler
//
//Texture2D with support of loading the data from file. 

#pragma once

//Is a Texture2D object at heart!
#include <Graphics/Texture2D/Texture2D.h>

namespace EngineAPI
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11TextureFile2D : public EngineAPI::Graphics::Texture2D
			{
			public:
				D3D11TextureFile2D() {};
				virtual ~D3D11TextureFile2D() = 0 {};

				//Inits the texture from file - DDS files only at the moment
				bool InitTexture2DFromDDSFile(EngineAPI::Graphics::GraphicsDevice* device,
					std::string ddsFilename,
					ResourceUsage textureUsage = RESOURCE_USAGE_IMMUTABLE,
					ResourceCPUAccessFlag textureCpuAccess = (ResourceCPUAccessFlag)0,
					ResourceBindFlag textureBindFlag = RESOURCE_BIND_SHADER_RESOURCE_BIT,
					std::string debugName = std::string(""));

				//Shutsdown the texture
				virtual void Shutdown() override;
			};
		}
	};
};