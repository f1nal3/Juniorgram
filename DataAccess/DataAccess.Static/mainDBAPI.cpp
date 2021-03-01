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

        const std::string_view columns
        {   
            "id integer, "
            "name varchar(32), "
            "age integer "
        };

        DBPostgre::PostgreTableOperations tbl{ psql };
        tbl.deleteTable("test"sv);
        tbl.createTable("test"sv, columns);

        DBPostgre::PostgreColumnOperations clmn{ psql };
        clmn.addNewColumn("test"sv, "password"sv, "varchar(32)");

        DBPostgre::PostgreFieldOperations fld{ psql };
        fld.insert("test"sv, "0, 'a', 25, 'a52b'");
        fld.insert("test"sv, "1, 'b', 31, 'b2c1'");
        fld.insert("test"sv, "2, 'c', 21, '24cb'");
        fld.insert("test"sv, "3, 'd', 33, '112a'");
        fld.insert("test"sv, "4, 'e', 30, '2421'");

        auto res = fld.select("test", "*");

        //suppressWarning(4239, -Winit-self)
        for (auto/*&*/ row : res)
        {
            std::cout << row.at("id").as<int>() << ' ';
            std::cout << row.at("name").as<std::string>() << ' ';
            std::cout << row.at("age").as<int>() << ' ';
            std::cout << row.at("password").as<std::string>() << '\n';
        }
        //restoreWarning

    }
    catch (const pqxx::sql_error& e)
    {
        std::cerr << e.what() << '\n';
        std::cerr << e.query() << '\n';
        std::cerr << e.sqlstate() << '\n';
        std::cin.get();
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cin.get();
        return 1;
    }
    
    std::cin.get();
    return 0;
}
restoreWarning

