#pragma once
#include <winsqlite/winsqlite3.h>

#include "LiteAdapter.hpp"
#include <DataAccess/AbstractRepositories.hpp>

namespace DataAccess
{
using Utility::DatabaseType;

struct LiteChannelsRepository final : ChannelsRepository<sqlite3>
{
    explicit LiteChannelsRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_LITE)
        : ChannelsRepository<sqlite3>(adapter, typeDB) {}

    ~LiteChannelsRepository() override = default;
};

struct LiteDirectMessageRepository final : DirectMessageRepository<sqlite3>
{
    explicit LiteDirectMessageRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_LITE)
        : DirectMessageRepository(adapter, typeDB) {}

    ~LiteDirectMessageRepository() override = default;
};

struct LiteLoginRepository final : LoginRepository<sqlite3>
{
    explicit LiteLoginRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_LITE)
        : LoginRepository<sqlite3>(adapter, typeDB) {}

    ~LiteLoginRepository() = default;
};

struct LiteMessagesRepository final : MessagesRepository<sqlite3>
{
    explicit LiteMessagesRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_LITE)
        : MessagesRepository<sqlite3>(adapter, typeDB) {}

    ~LiteMessagesRepository() override = default;
};

struct LiteRegisterRepository final : RegisterRepository<sqlite3>
{
    explicit LiteRegisterRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_LITE)
        : RegisterRepository<sqlite3>(adapter, typeDB) {}

    ~LiteRegisterRepository() override = default;
};

struct LiteRepliesRepository final : RepliesRepository<sqlite3>
{
    explicit LiteRepliesRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_LITE)
        : RepliesRepository<sqlite3>(adapter, typeDB) {}

    ~LiteRepliesRepository() override = default;
};
}  // namespace DataAccess
