#include <iostream>

#include "ArgumentParser.hpp"

int main(int argc, char** argv)
{ 
    using namespace StressTest;

    ArgumentParser parser(argc, argv);

    if (parser.isArggumentDefind(eKeys::count))
    {
        std::uint8_t a = parser.getArgument<eKeys::count, std::uint8_t>();
        a = 1;
        if (a > 0)
            std::cout << "!";
    }

    return EXIT_SUCCESS;
}
