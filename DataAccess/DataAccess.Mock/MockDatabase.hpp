#pragma once

#include <DataAccess/IAdapter.hpp>
#include <Utility/JGExceptions.hpp>

#include "MockDatabaseOptions.hpp"
#include "MockRepositoryInstance.hpp"

namespace MockObject
{
using DataAccess::IAdapter;
using MockObject::MockRepositoryInstance;

/** 
* @brief class MockDatabase.
* @detail This is a thread safe singleton. /
*         Inherited from IAdapter class - The abstract class controls /
*         the connection to the mock database.
*/
class MockDatabase final : public IAdapter
{
private:
    explicit MockDatabase(const std::string_view& options) : 
        _connection(std::make_unique<MockRepositoryInstance>(options)) {}
public:
    static std::shared_ptr<MockDatabase> Instance(const std::string_view& options);

    MockDatabase(const MockDatabase&)                  = delete;
    const MockDatabase& operator=(const MockDatabase&) = delete;

    MockDatabase(MockDatabase&&) noexcept            = delete;
    MockDatabase& operator=(MockDatabase&&) noexcept = delete;

    [[maybe_unused]] std::optional<std::any> query(const std::string_view& query) override;
    bool isConnected() const override;
    void closeConnection() override;

    virtual ~MockDatabase() { _instance.reset(); }

private:
    inline static std::shared_ptr<MockDatabase> _instance{};
    inline static std::mutex                    _staticMutex{};

    std::unique_ptr<MockRepositoryInstance>     _connection;
};
}  /// namespace MockObject
