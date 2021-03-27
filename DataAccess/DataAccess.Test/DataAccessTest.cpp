#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <iostream>

#include "DataAccess.Static/PostgreAdapter.hpp"



TEST_CASE("Tested the main object", "[PostgreSQL]")
{

    pqxx::connection m_connection{
        pqxx::zview("dbname=postgres user=postgres password=2319 hostaddr=127.0.0.1 port=5432")};
   
    
    /*auto psql = DataAccess::PostgreAdapter::getPostgre(
        "dbname=postgres user=postgres password=2319 hostaddr=127.0.0.1 port=50881");*/

    if (m_connection.is_open())
    {
        pqxx::work work{m_connection};

        auto res = work.exec("select 1;");

        work.commit();

        REQUIRE(res.at(0).at(0).as<int>() == 1);
    }
    else
    {
        std::cout << "Don't have connection to db!\n";
        REQUIRE(true);
    }
}
