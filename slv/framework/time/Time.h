#pragma once
#include "../../utils/time/Timer.h"

namespace slv
{
	// framework
	namespace frwk
	{
		namespace time
		{
			class Time
			{
				using Timer = slv::utils::time::Timer;
				static Timer s_timer;
				static float s_elapsedSec;
				static float s_prevTimeSec;
				static float s_deltaTimeSec;
			public:
				static void Init();
				static void Update();

				inline static float Elapsed() { return s_elapsedSec; }
				inline static float Delta() { return s_deltaTimeSec; }
			};
		}
	}
}