#include <iostream>
#include <thread>
#include <chrono>
    
#include "Tester.hpp"

int main(int argc, char** argv)
{ 
    using namespace PerformanceTest;

    try
    {
        ArgumentParser parser(argc, argv);

        if (parser.isArgumentDefind(eKeys::count) && parser.isArgumentDefind(eKeys::path))
        {
            Tester tester(parser);
            
            tester.run();
            tester.printResult();
        }
        else
        {
            throw std::runtime_error("Not all args were defind!");
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}
