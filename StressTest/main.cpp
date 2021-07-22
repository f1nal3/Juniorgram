#include <iostream>

#include "Tester.hpp"

int main(int argc, char** argv)
{ 
    using namespace StressTest;

    try
    {
        ArgumentParser parser(argc, argv);

        Pipe p("cls");
        p.write("cl");
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}
