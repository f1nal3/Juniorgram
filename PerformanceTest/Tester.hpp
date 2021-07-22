#pragma once
#include <iostream>

#include "ArgumentParser.hpp"
#include "Pipe.hpp"
#include "SimpleTimer.hpp"

namespace PerformanceTest
{
	class Tester
	{
	private:

		const std::uint8_t _testsCount;
		const fs::path     _instancePath;
		
		SimpleTimer  _timer;

	private:

		bool privateRunTest(const std::size_t testIndex);

	public:

		explicit Tester(const ArgumentParser& parser);

	public:

		bool run(void);

	};
}
