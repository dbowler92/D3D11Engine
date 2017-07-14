//CoreSubsystem.h
//Created 10/07/17
//Created By Daniel Bowler
//
//Base object for all subsystems

#pragma once

//Common system includes
#include <string>

//Common engine includes
#include "../../Debug/Log/DebugLog.h"

namespace EngineAPI
{
	namespace Core
	{
		class CoreSubsystem
		{
		public:
			CoreSubsystem() {};
			virtual ~CoreSubsystem() = 0 {};

			//Get the debug name of the subsystem
			std::string GetDebugName() { return _subsystemDebugNameString; };

			//Virtual functions that subclasses must implement
			//virtual void InitSubsystem() = 0 {}; //TODO
			virtual void ShutdownSubsystem() = 0 ;

			//Virtual functions that subclasses can implement

		protected:
			//Internally sets the debug name
			void SetDebugName(std::string s) { _subsystemDebugNameString = s; };

		private:
			//Debug string. 
			std::string _subsystemDebugNameString = "";
		};
	};
};