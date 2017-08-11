//DebugRendering.h
//Created 12/07/17
//Created By Daniel Bowler
//
//Debug rendering code will go here - eg: Drawing light volumes,
//vectors, lines, etc 

#pragma once

#include <Main/EngineStd.h>
#include <Config/EngineConfig.h>

//Device and manager
#include <Graphics/GraphicsManager/GraphicsManager.h>

namespace EngineAPI
{
	namespace Rendering
	{
		class VirtualCamera;

		class PointLight;
		class SpotLight;
	};
};

namespace EngineAPI
{
	namespace Debug
	{
		class DebugRendering
		{
			MAKE_STATICS_CLASS_ONLY(DebugRendering);

		public:
			//Debug renders the GBuffer. showPackedNormals == true, we will show
			//whats stored in the GBuffer [0-1]. If false, we will vis the unpacked normals
			//in the range [-1,1]
			static void DebugRenderGBuffersToScreen(bool showPackedNormals = true);

			//Debug renders a point light and spot light
			static void DebugRenderPointLight(EngineAPI::Rendering::PointLight* pointLight, EngineAPI::Rendering::VirtualCamera* mainCamera);
			static void DebugRenderSpotLight(EngineAPI::Rendering::SpotLight* spotLight, EngineAPI::Rendering::VirtualCamera* mainCamera);

		};
	};
};
