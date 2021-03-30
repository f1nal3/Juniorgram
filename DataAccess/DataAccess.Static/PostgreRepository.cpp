#include "PostgreRepository.hpp"
#include "Utility/Exception.hpp"

#include <iostream>


using namespace DataAccess;

PostgreRepository::PostgreRepository()
{
    m_postgreInstance = PostgreAdapter::getPostgre(
        "dbname=postgres user=postgres password=Asw789654 hostaddr=127.0.0.1 port=5432");
    //TODO Import parameters from argument parser
}

std::vector<std::string> PostgreRepository::getAllChannelsList()
{
    std::vector<std::string> result;
    if (m_postgreInstance->isConnected())
    {
        auto channelListRow =
            m_postgreInstance->query("SELECT channel_name FROM channels;");
        
        for (auto item : channelListRow.value())
        {
            std::cout << item.at(0).c_str() <<'\n';
            result.emplace_back(std::string(item.at(0).c_str()));
        }

    }
    return result;
}

std::vector<std::string> PostgreRepository::getMessageHistoryForUser(std::string userID)
{
    std::vector<std::string> result;
    if (m_postgreInstance->isConnected())
    {
        auto messageHistory = 
            m_postgreInstance->query("SELECT messages FROM message_history WHERE user_name = " + userID);

        for (auto item : messageHistory.value())
        {
            result.emplace_back(std::string(item.at(0).c_str()));
        }
    }
    return result;
}

void PostgreRepository::storeMessages(std::vector<std::string>)
{
    throw Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);
}
