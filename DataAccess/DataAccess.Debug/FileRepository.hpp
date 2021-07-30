#pragma once

#include "FileDB.hpp"
#include "DataAccess/IRepository.hpp"

namespace DataAccess
{
class FileRepository : public IRepository
{
   public:
	FileRepository();
	virtual ~FileRepository() = default;

	std::vector<Network::ChannelInfo> getAllChannelsList() override;
	std::vector<Network::MessageStoringInfo> getMessageHistoryForUser(const std::uint64_t channelID) override;

private:
	std::unique_ptr<FileDB> database;
};
}
