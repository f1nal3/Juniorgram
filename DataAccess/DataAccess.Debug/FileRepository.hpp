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
	virtual std::vector<std::string> getMessageHistoryForUser(std::string userID) override;
	virtual void storeMessages(std::vector<std::string> messageList) override;

private:
	std::unique_ptr<FileDB> database;
};
}
