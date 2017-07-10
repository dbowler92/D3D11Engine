//CoreObject.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Base object for all objects in the engine && game. Currently, does very little.

#pragma once

//Common system includes
#include <string>

//Common engine includes
#include "../../Debug/Log/DebugLog.h"

namespace EngineAPI
{
	namespace Core
	{
		class CoreObject
		{
		public:
			CoreObject() {};
			virtual ~CoreObject() = 0 {  };

			//Get the debug name
			std::string GetDebugName() { return _debugNameString; };

		protected:
			//Internally sets the debug name
			void SetDebugName(std::string s) { _debugNameString = s; };

		private:
			//Debug string. 
			std::string _debugNameString = "";
		};
	};
};