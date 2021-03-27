#include "PostgreRepository.hpp"
#include "Utility/Exception.hpp"

using namespace DataAccess;

PostgreRepository::PostgreRepository()
{}

std::vector<std::string> PostgreRepository::getAllChannelsList(std::uint64_t userID)
{
    throw Utility::NotImplementedException("Not implemented.", __FILE__, userID);
}

std::vector<std::string> PostgreRepository::getMessageHistoryForUser(std::string)
{
    throw Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);
}

void PostgreRepository::storeMessages(std::vector<std::string>)
{
    throw Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);
}
