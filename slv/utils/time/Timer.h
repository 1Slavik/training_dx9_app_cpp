#pragma once

#include <chrono>

namespace slv
{
	namespace utils
	{
		namespace time
		{
			class Timer
			{
			private:
				using clock_t = std::chrono::steady_clock;
				std::chrono::time_point<clock_t> m_start;

			public:
				Timer() : m_start(clock_t::now())
				{}

				inline void Reset() { m_start = clock_t::now(); }
				inline float ElapsedMs()
				{
					return std::chrono::duration<float, std::milli>(clock_t::now() - m_start).count();
				}
			};
		}
	}
}