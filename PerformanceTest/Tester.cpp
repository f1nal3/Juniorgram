#include "Tester.hpp"

namespace PerformanceTest
{
    const char** Tester::privateCreateArgs(void) const
    {
        std::array<const char*, 5> args = { "cl", "mh", "sm", "p", "q" };
        std::default_random_engine engine;
        std::shuffle(args.begin(), args.end(), engine);

        return args.data();
    }

	Tester::Tester(const ArgumentParser& parser)
        : _testsCount{ parser.getCountArgument() }, _instancePath{ parser.getPathArgument() }, _timestamps(_testsCount) {}

	void Tester::run()
    {
        [[maybe_unused]] const char** argv = privateCreateArgs();

        for (std::size_t testIndex = 0; testIndex < _testsCount; testIndex++)
        {
            _timer.start();

        #ifdef _WIN32
            
            ::STARTUPINFO         si = { sizeof(STARTUPINFO) };
            ::PROCESS_INFORMATION pi;

            // Creating testing object.
            ::CreateProcess(_instancePath.string().c_str(), " cl sm q", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

            // Waiting until testing object finish it's work.
            ::WaitForSingleObject(pi.hProcess, INFINITE);

            // Closing handles (Don't know for what purposes but VS throwing exceptions).
            ::CloseHandle(pi.hProcess);
            ::CloseHandle(pi.hThread);

        #elif



        #endif

            _timer.stop();

            _timestamps.at(testIndex) = _timer.elapsed();
        }
    }

    void Tester::printResult(void) const
    {
        std::size_t testIndex = 1;
        for (const auto timestamp : _timestamps)
        {
            std::cout << std::fixed << std::setprecision(std::streamsize(9))
                      << "Test #" << std::setw(3) << testIndex++ << ": "
                      << std::setw(15) << timestamp << "ns\n";
        }

        std::cout << "\nAverage time:  " << std::accumulate(_timestamps.begin(), _timestamps.end(), 0.0) / _timestamps.size() << "ns" << std::endl;
    }
}
