#include "PostgreRepository.hpp"
#include "Utility/Exception.hpp"

#include <iostream>


using namespace DataAccess;

PostgreRepository::PostgreRepository()
{}

std::vector<std::string> PostgreRepository::getAllChannelsList()
{
    auto psql = mp_dataBaseInstance->getPostgre(
        "dbname=postgres user=postgres hostaddr=127.0.0.1 port=5432");

    std::vector<std::string> result;
    if (psql->getConnection().is_open())
    {
        auto channelListRow =
            psql->query("SELECT channel_name FROM channels;");
        
        for(auto item : channelListRow.value())
        {
            std::cout << item.at(0).c_str() <<'\n';
            result.emplace_back(std::string(item.at(0).c_str()));
        }

    }
    return result;
}

std::vector<std::string> PostgreRepository::getMessageHistoryForUser(std::string)
{
    throw Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);
}

void PostgreRepository::storeMessages(std::vector<std::string>)
{
    throw Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);
}
