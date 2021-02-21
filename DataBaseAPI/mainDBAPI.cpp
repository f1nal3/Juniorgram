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


        DBPostgre::PostgreTableOperations tb(psql);
        //tb.createTable("test"sv, "id integer"sv);

        DBPostgre::PostgreColumnOperations col(psql);
        //col.addNewColumn("test"sv, "name"sv, "varchar(4)"sv);

        DBPostgre::PostgreFieldOperations fld(psql);
        //fld.insert("test"sv, "\'aaa\', 0", "name, id"sv);

        std::cout << psql.use_count() << std::endl;
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