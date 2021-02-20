#include <iostream>

#include "../common/WarningSuppressing.h"

#include "Headers/DataBasePostgreSQL.hpp"

suppressWarning(4099, -Winit-self)
int main()
{   
    using namespace std::literals::string_view_literals;

    try
    {

        auto psql = DBPostgre::PostgreSQL::getPostgre(
            "dbname=postgres user=postgres password=Asw789654 hostaddr=127.0.0.1 port=5432");

        std::cout << "Count: " << psql.use_count() << std::endl;

        DBPostgre::PostgreTableOperations tb(psql);

        std::cout << "Count: " << psql.use_count() << std::endl;
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << '\n';
        std::cin.get();
        return 1;
    }
    
    std::cin.get();
    return 0;
}
restoreWarning