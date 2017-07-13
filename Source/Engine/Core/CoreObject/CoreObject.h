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
			virtual ~CoreObject() = 0 {};

			//Get the debug name of any object
			std::string GetDebugName() { return _debugNameString; };

			//Virtual functions that subclasses must implement
		
			//Virtual functions that subclasses can implement
			virtual void Shutdown() {};

		protected:
			//Internally sets the debug name - can be override by, say, D3D11
			//resources so they can set the debug name of the ID3D11Resource/
			//ID3D11DeviceChild
			virtual void SetDebugName(std::string s) { _debugNameString = s; };

		private:
			//Debug string. 
			std::string _debugNameString = "";
		};
	};
};