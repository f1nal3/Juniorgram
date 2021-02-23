#include <iostream>

#include "../common/WarningSuppressing.h"

#include "Headers/DataBasePostgreSQL.hpp"

suppressWarning(4099, -Winit-self)
int main()
{   
    try
    {
        using namespace std::literals::string_view_literals;

        auto psql = DBPostgre::PostgreSQL::getPostgre(
            "dbname=postgres user=postgres password=Asw789654 hostaddr=127.0.0.1 port=5432");
        psql->getConnection().set_client_encoding("WIN866");

        DBPostgre::PostgreFieldOperations fld(psql);

        std::cout << std::get<0>(psql->quary("SELECT 1").value()[0].as<int>()) << '\n';

        std::cout << std::get<0>(fld.select("test", "*", "", 5)[1].as<int>()) << '\n';
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