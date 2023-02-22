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
public:
    static std::shared_ptr<MockDatabase> Instance(const std::string_view& options);
 
    explicit MockDatabase(const std::string_view& options) :
        _connection(std::make_unique<MockRepositoryInstance>(options)) 
    {    
    }

    MockDatabase(const MockDatabase& other)            = delete;
    MockDatabase& operator=(const MockDatabase& other) = delete;

    MockDatabase(MockDatabase&& other)            = delete;
    MockDatabase& operator=(MockDatabase&& other) = delete;

    std::optional<std::any> query(const std::string_view& query) override;

    bool isConnected() const override;

    void closeConnection() override;

    ~MockDatabase() override = default;

private:
    inline static std::shared_ptr<MockDatabase> _instance{};
    std::unique_ptr<MockRepositoryInstance>     _connection{};
};
}  /// namespace MockObject
