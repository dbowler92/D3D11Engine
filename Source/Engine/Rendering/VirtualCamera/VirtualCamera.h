//VirtualCamera.h
//Created 17/07/17
//Created By Daniel Bowler
//
//Super simple virtual camera (Fly camera) - this will be replaced
//later with components && a much better implementation. But for now, 
//a quick camera implementation is required. 

#pragma once

//Parent
#include <Core/Coreobject/CoreObject.h>

namespace EngineAPI
{
	namespace Rendering
	{
		class VirtualCamera : public EngineAPI::Core::CoreObject
		{
		public:
			VirtualCamera() {};
			virtual ~VirtualCamera() {};

			//Shutdown
			virtual void Shutdown() override;

		public:
			//Inits the camera
			virtual void Init();
		};
	}
}