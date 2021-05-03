#pragma once

#include "FileDB.hpp"
#include "DataAccess\IRepository.hpp"

namespace DataAccess
{
class FileRepository : public IRepository
{
   public:
	FileRepository();
	virtual ~FileRepository() = default;

	virtual std::vector<std::string> getAllChannelsList() override;
	virtual std::vector<std::string> getMessageHistoryForUser(const unsigned channelID) override;
	virtual void storeMessage(const Network::MessageInfo& messageInfo, const unsigned channelID) override;

private:
	std::unique_ptr<FileDB> database;
};
}
