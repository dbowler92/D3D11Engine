//GameTimer.h
//Created 26/07/16
//Created By Daniel Bowler
//
//W32 timer

#pragma once

//Parent
//#include "../../Core/CoreObject/CoreObject.h"

namespace EngineAPI
{
	namespace Utils
	{
		class GameTimer /*: public EngineAPI::Core::CoreObject*/
		{
		public:
			GameTimer();

			float TotalTime()const;  // in seconds
			float DeltaTime()const; // in seconds

			void Reset(); // Call before message loop.
			void Start(); // Call when unpaused.
			void Stop();  // Call when paused.
			void Tick();  // Call every frame.

		private:
			double mSecondsPerCount;
			double mDeltaTime;

			__int64 mBaseTime;
			__int64 mPausedTime;
			__int64 mStopTime;
			__int64 mPrevTime;
			__int64 mCurrTime;

			bool mStopped;
		};
	};
};