//GraphicsManager.h
//Created 09/07/17
//Created By Daniel Bowler
//
//Represents the graphics subsystem. 

#pragma once

#include "../../Config/EngineConfig.h" //Build settings

#ifdef ENGINE_CONFIG_GRAPHICS_API_D3D11
#include "D3D11\D3D11GraphicsManager.h"
#endif

//Forward declarations
namespace EngineAPI
{
	namespace OS
	{
		namespace Platform
		{
			class OS_PLATFORM_IMPLEMENTATION_CLASS_NAME(Application);
		};
	};
};

namespace EngineAPI
{
	namespace Graphics
	{
		class GraphicsManager : public RENDERING_PLATFORM_IMPLEMENTATION(GraphicsManager)
		{
			//Only application should be able to create/destroy this
			//subsystem + pass events. 
			friend class EngineAPI::OS::Platform::OS_PLATFORM_IMPLEMENTATION_CLASS_NAME(Application);
			
		public:
			//Graphics manager is a singleton
			static GraphicsManager* GetInstance();

		protected:
			GraphicsManager() {};
			~GraphicsManager() {};

			//Called after shutting down the API specific subsystem 
			//by application
			static void DestroyInstance();

		private:
			static GraphicsManager* instance;
		};
	};
};