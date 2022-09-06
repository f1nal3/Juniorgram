#pragma once

#include <DataAccess/AbstractRepositories.hpp>

#include "PostgreQuery.hpp"
#include "UsersAmountFinder.hpp"

namespace DataAccess
{
    using Utility::DatabaseType;

struct PostgreChannelsRepository final : ChannelsRepository<pqxx::result>
{
    explicit PostgreChannelsRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_POSTGRE) 
        : ChannelsRepository<pqxx::result>(adapter,typeDB) {}
    ~PostgreChannelsRepository() override = default;
};

struct PostgreDirectMessageRepository final : DirectMessageRepository<pqxx::result>
{
    explicit PostgreDirectMessageRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_POSTGRE)
        : DirectMessageRepository(adapter,typeDB) {}

    ~PostgreDirectMessageRepository() override = default;
};

struct PostgreLoginRepository final : LoginRepository<pqxx::result>
{
    explicit PostgreLoginRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_POSTGRE)
        : LoginRepository<pqxx::result>(adapter,typeDB) {}
    
    ~PostgreLoginRepository() = default;
};

struct PostgreMessagesRepository final : MessagesRepository<pqxx::result>
{
    explicit PostgreMessagesRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_POSTGRE)
        : MessagesRepository<pqxx::result>(adapter,typeDB) {}
    
    ~PostgreMessagesRepository() override = default;
};

struct PostgreRegisterRepository final : RegisterRepository<pqxx::result>
{
    explicit PostgreRegisterRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_POSTGRE)
        : RegisterRepository<pqxx::result>(adapter,typeDB) {}

    ~PostgreRegisterRepository() override = default;
};

struct PostgreRepliesRepository final : RepliesRepository<pqxx::result>
{
    explicit PostgreRepliesRepository(const std::shared_ptr<IAdapter>& adapter, DatabaseType typeDB = DatabaseType::DB_POSTGRE)
        : RepliesRepository<pqxx::result>(adapter,typeDB) {}

    ~PostgreRepliesRepository() override = default;
};
}  // namespace DataAccess
