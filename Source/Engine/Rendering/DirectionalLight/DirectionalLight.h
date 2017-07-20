//DirectionalLight.h
//Created 20/07/17
//Created By Daniel Bowler
//
//TEMP: Until I have a component system. This represents
//a directional light source in the scene

#pragma once

//Parent
#include <Core/CoreObject/CoreObject.h>

//Math
#include <Includes/MathIncludes.h>

//Data representing the light
struct DirectionalLightGraphicsData
{
	XMFLOAT4 LightDirection;
	XMFLOAT4 LightColourAndIntensity;
};

namespace EngineAPI
{
	namespace Rendering
	{
		namespace Lights
		{
			class DirectionalLight : public EngineAPI::Core::CoreObject
			{
			public:
				DirectionalLight() {};
				~DirectionalLight() {};

				//Override the shutdown function
				void Shutdown() override;

				//Inits the directional light with initial data
				void InitDirectionalLightSource(XMFLOAT3 dir, 
					XMFLOAT3 col, float intensity, 
					std::string debugName = "");

				//Set active state of light - renders only when
				//true
				void SetActiveState(bool state) { isActive = state; };

				//Renders the directional light source during the 
				//lighting pass
				void Render();

			protected:
				//Light data
				DirectionalLightGraphicsData lightData;

				//Is light active?
				bool isActive = false;
			};
		};
	};
};