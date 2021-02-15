#include <iostream>

#include "../common/WarningSuppressing.h"

#include <pqxx/pqxx>

suppressWarning(4099, -winit-self)
int main()
{   
    try
    {
        pqxx::connection c("dbname=test user=postgres password=123 hostaddr=127.0.0.1 port=5432");

        pqxx::work w(c);

        pqxx::row r = w.exec1("SELECT 1");

        w.commit();

        std::cout << r[0].as<int>() << std::endl;
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
restoreWarning