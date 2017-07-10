//CoreObject.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Base object for all objects in the engine && game. Currently, does very little.

#pragma once

//Common includes
#include <string>

namespace EngineAPI
{
	namespace Core
	{
		class CoreObject
		{
		public:
			CoreObject() {};
			virtual ~CoreObject() = 0 {};

			//Get the debug name
			std::string GetDebugName() { return debugNameString; };

		protected:
			//Debug string. 
			std::string debugNameString;

		protected:
			//Internally sets the debug name
			void SetDebugName(std::string s) { debugNameString = s; };
		};
	};
};