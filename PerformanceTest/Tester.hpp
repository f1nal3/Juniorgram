#pragma once
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>

#include "ArgumentParser.hpp"
#include "SimpleTimer.hpp"

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <process.h>
#endif

namespace PerformanceTest
{
	class Tester
	{
	private:

		const std::uint8_t _testsCount;
		const fs::path     _instancePath;
		
		std::vector<double> _timestamps;

		SimpleTimer  _timer;

	private:

		std::vector<const char*> privateCreateArgs(void) const;

	public:

		explicit Tester(const ArgumentParser& parser);

	public:

		void run(void);

		void printResult(void) const;
	};
}
