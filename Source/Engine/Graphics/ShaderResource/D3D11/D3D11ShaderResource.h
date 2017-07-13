//D3D11ShaderResource.h
//Created 11/07/17
//Created By Daniel Bowler
//
//Represents a single shader (unlinked) loaded from file - HLSL Shaders can be
//compiled by FXC as part of the VS2015 build process 

#pragma once

//Parent class
#include "../../../Core/CoreObject/CoreObject.h"

//Graphics includes - inc D3D11
#include "../../Includes/GraphicsIncludes.h"

//Device used to create texture, buffers && views
#include "../../Graphics/GraphicsDevice/GraphicsDevice.h"

//Statics (D3D11)
#include "../../../Statics/D3D11Statics/D3D11Statics.h"

namespace EngineAPI 
{
	namespace Graphics
	{
		namespace Platform
		{
			class D3D11ShaderResource : public EngineAPI::Core::CoreObject
			{
			public:
				D3D11ShaderResource() {};
				virtual ~D3D11ShaderResource() = 0 {};

				//Shutsdown the base shader resource. 
				virtual void Shutdown() override = 0;

			protected:
				//Parses a shader file in to a buffer
				bool ReadCompiledShaderFile(const char* shaderFile);

				//Cleansup the parsed shader bytecode once we have done with it
				void CleanupBytecodeBuffer();

				//Returns a pointer to the internal bytecode buffer. Null if
				//it doesnt exist
				char* GetShaderBytecodeBuffer() { return shaderBytecodeBuffer; };
				SIZE_T GetShaderBytecodeBufferSize() { return shaderBytecodeSize; };

			private:
				//Parsed shader. 
				char* shaderBytecodeBuffer = nullptr;
				SIZE_T shaderBytecodeSize = 0;
			};
		};
	};
};