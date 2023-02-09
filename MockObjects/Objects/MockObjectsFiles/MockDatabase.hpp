#pragma once

#include <DataAccess/IAdapter.hpp>
#include <Utility/JGExceptions.hpp>
#include "MockDatabaseOptions.hpp"
#include "MockRepositoryInstance.hpp"

namespace MockDatabase
{
using DataAccess::IAdapter;
using MockObject::MockRepositoryInstance;

/** 
* @brief class MockDatabase.
* @detail This is a thread safe singleton. /
*         Inherited from IAdapter class - The abstract class controls /
*         the connection to the database.
*/
class MockDatabase final : public IAdapter
{
public:
    static std::shared_ptr<MockDatabase> Instance([[maybe_unused]] const std::string_view& options = {});
 
    explicit MockDatabase(const std::string_view& options) :
        _connection(std::make_unique<MockRepositoryInstance>(options)) {}

    MockDatabase(const MockDatabase& other)            = delete;
    MockDatabase& operator=(const MockDatabase& other) = delete;

    MockDatabase(MockDatabase&& other)            = delete;
    MockDatabase& operator=(MockDatabase&& other) = delete;

    ~MockDatabase() override = default;

    std::optional<std::any> query(const std::string_view& query) override;

    bool isConnected() const override;

    void closeConnection() override;

private:
    inline static std::mutex                    _staticMutex{};
    inline static std::shared_ptr<MockDatabase> _instance{};

    std::mutex                                  _queryMutex;
    std::unique_ptr<MockRepositoryInstance>     _connection;
};
}  // namespace MockDatabase
