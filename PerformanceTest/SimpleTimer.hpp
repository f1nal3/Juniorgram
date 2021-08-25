#pragma once
#include <chrono>

namespace PerformanceTest
{
	class SimpleTimer
	{
		using hsc = std::chrono::high_resolution_clock;

        private:

			hsc::time_point	_start;
            hsc::time_point	_end;

        public:

			SimpleTimer() = default;
			~SimpleTimer() = default;

			void start(void) noexcept;
            void stop(void) noexcept;
			void reset(void) noexcept;

			double elapsed(void) const noexcept;
	};
}
