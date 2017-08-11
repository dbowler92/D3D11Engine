//SpotLight.h
//Created 27/07/17
//Created By Daniel Bowler
//
//Represents a point light

#pragma once

//Parent
#include <Core/CoreObject/CoreObject.h>

//Math
#include <Includes/MathIncludes.h>

//Data representing the light
struct SpotLightGraphicsData
{
	XMFLOAT3   LightPosition;	
	float	   LightRange;

	XMFLOAT3   LightDirection;
	float      LightOuterAngle;

	float      LightInnerAngle;
	XMFLOAT3   __PadF3;

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
		class SpotLight : public EngineAPI::Core::CoreObject
		{
		public:
			SpotLight() {};
			~SpotLight() {};

			//Override the shutdown function
			void Shutdown() override;

			//Inits the spot light with initial data
			void InitSpotLightSource(XMFLOAT3 position, float range, 
				XMFLOAT3 rotationTransformRollPitchYawDegrees, 
				float lightOuterAngleDeg, float lightInnerAngleDeg,
				XMFLOAT3 col, float intensity,
				std::string debugName = "");

			//Set active state of light - renders only when
			//true
			void SetActiveState(bool state) { isActive = state; };

			//Renders the point light source during the 
			//lighting pass
			void Render(EngineAPI::Rendering::VirtualCamera* mainCamera);

		public:
			//Getters and setters for the spot light data
			float GetOuterAngle() { return lightData.LightOuterAngle; };
			void SetOuterAngle(float outerAngleDeg) { lightData.LightOuterAngle = outerAngleDeg; };
			
			float GetInnerAngle() { return lightData.LightInnerAngle; };
			void SetInnerAngle(float innerAngleDeg) { lightData.LightInnerAngle = innerAngleDeg; };

			XMFLOAT3 GetPosition() { return lightData.LightPosition; };
			void SetPosition(XMFLOAT3 p) { lightData.LightPosition = p; };
			void SetPosition(float x, float y, float z) { XMFLOAT3 p(x, y, z); lightData.LightPosition = p; };

		protected:
			//Light data
			SpotLightGraphicsData lightData;

			//Rotation info
			XMFLOAT3 lightRotationRollPitchYawDegrees;

			//Is light active?
			bool isActive = false;
		};
	};
};