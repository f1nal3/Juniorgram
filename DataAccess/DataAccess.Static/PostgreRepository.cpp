#include "PostgreRepository.hpp"
#include "Utility/Exception.hpp"

#include <iostream>


using namespace DataAccess;

PostgreRepository::PostgreRepository()
{}

std::vector<std::string> PostgreRepository::getAllChannelsList()
{
    std::cout << "Get some error at getAllChannelList()";


    auto psql = mp_dataBaseInstance->getPostgre(
        "dbname=postgres user=postgres password=2319 hostaddr=127.0.0.1 port=5432");

    if (psql->getConnection().is_open())
    {
        pqxx::row channeList =
            psql->query("SELECT * FROM public.user_ids_n_channels;").value().at(0);

        return {channeList["channels"].c_str()};
    }
    else
    {
        std::cout << "Don't have connection to db!\n";
    }

    throw Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);
}

std::vector<std::string> PostgreRepository::getMessageHistoryForUser(std::string)
{
    throw Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);
}

void PostgreRepository::storeMessages(std::vector<std::string>)
{
    throw Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);
}
