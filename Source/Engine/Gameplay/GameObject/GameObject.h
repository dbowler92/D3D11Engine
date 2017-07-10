//GameObject.h
//Created 30/06/17
//Created By Daniel Bowler
//
//Game object that will be added to a scenes scene graph. Acts as a container
//for a set of components. Has its own global transform

#pragma once

//Parent
#include "../../Core/CoreObject/CoreObject.h"

namespace EngineAPI
{
	namespace Gameplay
	{
		class GameObject : public EngineAPI::Core::CoreObject
		{
		public:
			GameObject();
			~GameObject();

		};
	};
};
