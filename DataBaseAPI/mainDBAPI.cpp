#include <iostream>

#include "../common/WarningSuppressing.h"

#include <pqxx/pqxx>

suppressWarning(4099, -winit-self)
int main()
{   
    pqxx::result res;

    std::cin.get();
    return 0;
}
restoreWarning