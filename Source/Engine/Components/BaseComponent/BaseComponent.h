//BaseComponent.h
//Created 30/06/17
//Created By Daniel Bowler
//
//Interface class that all components will inherit from

#pragma once

//Parent
#include "../../Core/CoreObject/CoreObject.h"

namespace EngineAPI
{
	namespace Component
	{
		class BaseComponent : public EngineAPI::Core::CoreObject
		{
		public:
			BaseComponent() {};
			virtual ~BaseComponent() = 0 {};
		};
	}
};

