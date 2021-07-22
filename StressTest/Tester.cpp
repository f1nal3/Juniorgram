#include "Tester.hpp"

namespace StressTest
{
	bool Tester::privateRunTest(const std::size_t testIndex)
	{
		return testIndex;
	}

	Tester::Tester(const ArgumentParser& parser)
		: _testsCount{ parser.getCountArgument() }, _instancePath{ parser.getPathArgument() } {}

	bool Tester::run()
	{
		for (std::size_t testIndex = 0; testIndex < _testsCount; testIndex++)
		{
			this->privateRunTest(testIndex);
		}

		return true;
	}
}
