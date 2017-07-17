//VirtualCamera.h
//Created 17/07/17
//Created By Daniel Bowler
//
//Super simple virtual camera (Fly camera) - this will be replaced
//later with components && a much better implementation. But for now, 
//a quick camera implementation is required. 
//
//NOTE: LH camera

#pragma once

//Parent
#include <Core/Coreobject/CoreObject.h>

//Math
#include <Includes/MathIncludes.h>

namespace EngineAPI
{
	namespace Rendering
	{
		class VirtualCamera : public EngineAPI::Core::CoreObject
		{
		public:
			VirtualCamera();
			virtual ~VirtualCamera() {};

			//Shutdown
			void Shutdown() override;

			//Set camera view properties at init time (or when the screen resizes in the case
			//of the camera projection properties
			void InitCameraViewProperties(const XMFLOAT3& cameraPosition, const XMFLOAT3& cameraTarget, const XMFLOAT3& worldUp);
			void InitCameraPerspectiveProjectionProperties(float fovYDegrees, float aspect, float nearZ, float farZ);

			//Strafe/Walk the camera a distance.
			void Strafe(float distance);
			void Walk(float distance);

			//Rotate the camera (pitch/Yaw)
			void AddPitch(float angleXDegrees);
			void AddYaw(float angleYDegrees);

			//Rebuilds the view matrix 
			void RebuildView();
			
		public:
			//Getters
			XMMATRIX GetView() { return XMLoadFloat4x4(&view); };
			XMMATRIX GetProj() { return XMLoadFloat4x4(&proj); };
			XMMATRIX GetViewProj() { return GetView() * GetProj(); };

		protected:
			//View and projection matricies
			XMFLOAT4X4 view;
			XMFLOAT4X4 proj;
			
			//Camera data
			XMFLOAT3 position;
			XMFLOAT3 right;
			XMFLOAT3 up;
			XMFLOAT3 look;

			//Perspective projection data
			float fovYRads;
			float aspect;
			float nearZ;
			float farZ;

			//Is perspective or othogonal projection?
			bool isPerspectiveMatrix = false;
		};
	}
}