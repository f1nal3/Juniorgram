#include "SimpleTimer.hpp"

namespace PerformanceTest
{
	void SimpleTimer::start() noexcept 
	{
		_start = hsc::now();
	}

	void SimpleTimer::stop() noexcept 
	{ 
		_end = hsc::now();
	}

	void SimpleTimer::reset() noexcept 
	{ 
		_start        = {};
        _end          = {};
	}

	double SimpleTimer::elapsed() const noexcept 
	{
		return (_start - _end).count() / 1'000'000'000.0;
	}
}
