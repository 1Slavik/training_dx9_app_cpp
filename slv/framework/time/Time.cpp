#include "Time.h"

using Time = slv::frwk::time::Time;

Time::Timer Time::s_timer = Time::Timer();
float Time::s_elapsedSec = 0.0f;
float Time::s_prevTimeSec = 0.0f;
float Time::s_deltaTimeSec = 0.0f;

void Time::Init()
{
	s_elapsedSec = s_timer.ElapsedMs() / 1000.0f;
	s_prevTimeSec = s_elapsedSec;
}
void Time::Update()
{
	s_elapsedSec = s_timer.ElapsedMs() / 1000.0f;
	s_deltaTimeSec = s_elapsedSec - s_prevTimeSec;
	s_prevTimeSec = s_elapsedSec;
}