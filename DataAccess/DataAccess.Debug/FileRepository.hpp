#pragma once
#include <DataAccess/IRepository.hpp>

#include "FileDB.hpp"

namespace DataAccess
{
class FileRepository : public IRepository
{
   public:
	FileRepository();
	virtual ~FileRepository() = default;

	std::vector<Base::Models::ChannelInfo> getAllChannelsList() override;
	std::vector<Base::Models::MessageInfo> getMessageHistoryForUser(const std::uint64_t channelID) override;

private:
	std::unique_ptr<FileDB> _database;
};
}
