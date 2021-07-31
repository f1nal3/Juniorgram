#include <iostream>
#include <thread>
#include <chrono>

#include "Tester.hpp"

int main(int argc, char** argv)
{ 
    using namespace PerformanceTest;
    using namespace std::literals;

    try
    {
        ArgumentParser parser(argc, argv);

        FILE* f = _popen("start Client.CLI.exe", "r");



        _pclose(f);

    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}
