#include "Tester.hpp"

namespace PerformanceTest
{
    std::vector<const char*> Tester::privateCreateArgs(void) const
    {
        std::array<const char*, 4> argTypes = { "q ", "mh ", "p ", "cl " };
        std::vector<const char*> args;

        std::uniform_int_distribution distIndex(1, 3), distRange(25, 75);

        std::random_device rd;
        std::mt19937 engine(rd());

        auto rndIndex = std::bind(distIndex, engine);
        auto rndRange = std::bind(distRange, engine);

        std::size_t range = rndRange();

        for (std::size_t randArgIter = 0; randArgIter < range; randArgIter++)
        {
            const char* argType = argTypes[randArgIter < 25 ? rndIndex() : rndIndex() - 1];
            if (argType[0] == 'q')
            {
                args.push_back("cl ");
                args.push_back("mh ");
                args.push_back("q");
                break;
            }

            args.push_back(argType);
        }

        return args;
    }

	Tester::Tester(const ArgumentParser& parser)
        : _testsCount{ parser.getCountArgument() }, _instancePath{ parser.getPathArgument() }, _timestamps(_testsCount) {}

	void Tester::run()
    {
        std::vector<const char*> argv;

        for (std::size_t testIndex = 0; testIndex < _testsCount; testIndex++)
        {
            // Creating for each iteration new set of args.
            argv = privateCreateArgs();

        #ifdef _WIN32

            // This needs because 'CreateProcess' needs args by one string not 'const char**' like spawnv.
            std::string argvByOneString;
            argvByOneString = std::accumulate(argv.begin(), argv.end(), std::string{});

            std::cout << "Args to testing objects: " << argvByOneString << std::endl;

        #endif

            _timer.start();

        #ifdef _WIN32
            // Unfortuaetly, 'spawnv' func doesn't work correctly on Windows
            // That I use WinAPI.

            ::STARTUPINFO         si = { sizeof(::STARTUPINFO) };
            ::PROCESS_INFORMATION pi;

            // Creating testing object.
            //              Not '_instancePath.c_str()' because it returns 'value_type' which is 'wchar_t' by default.
            ::CreateProcess(_instancePath.string().c_str(),
                const_cast<LPSTR>(argvByOneString.c_str()),
                NULL, NULL, FALSE,
                CREATE_NEW_CONSOLE, NULL, NULL,
                &si, &pi);

            // Waiting until testing object finishes it's work.
            ::WaitForSingleObject(pi.hProcess, INFINITE);

            // Closing handles (Don't know for what purposes but VS throwing exceptions).
            ::CloseHandle(pi.hProcess);
            ::CloseHandle(pi.hThread);

        #elif

            // Creating testing object and waiting until it finishes.
            spawnv(P_WAIT, _instancePath.string(), argv.data());

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
                      << std::setw(15) << timestamp - 5 << "s\n";
        }

        std::cout << "\nAverage time:  " << std::accumulate(_timestamps.begin(), _timestamps.end(), 0.0) / _timestamps.size() - 5 << "s" << std::endl;
    }
}
