//PointLight.h
//Created 25/07/17
//Created By Daniel Bowler
//
//Represents a point light

#pragma once

//Parent
#include <Core/CoreObject/CoreObject.h>

//Math
#include <Includes/MathIncludes.h>

//Data representing the light
struct PointLightGraphicsData
{
	XMFLOAT3   LightPosition;	
	float	   LightRange;
	XMFLOAT4   LightColourAndIntensity;
	XMFLOAT4X4 LightWorldViewProjection;
};

namespace EngineAPI
{
	namespace Rendering
	{
		class VirtualCamera;
	};
};

namespace EngineAPI
{
	namespace Rendering
	{
		class PointLight : public EngineAPI::Core::CoreObject
		{
		public:
			PointLight() {};
			~PointLight() {};

			//Override the shutdown function
			void Shutdown() override;

			//Inits the point light with initial data
			void InitPointLightSource(XMFLOAT3 position, 
				float range, XMFLOAT3 col, float intensity,
				std::string debugName = "");

			//Set active state of light - renders only when
			//true
			void SetActiveState(bool state) { isActive = state; };

			//Renders the point light source during the 
			//lighting pass
			void Render(EngineAPI::Rendering::VirtualCamera* mainCamera);

		protected:
			//Light data
			PointLightGraphicsData lightData;

			//Is light active?
			bool isActive = false;
		};
	};
};