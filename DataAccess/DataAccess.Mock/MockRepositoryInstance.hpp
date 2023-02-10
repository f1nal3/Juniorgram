#pragma once 

#include "MockRepositoryStorage.hpp"

namespace MockObject
{
/**
* @brief MockRepositoryInstance class.
* @details Designed to initialize database options and tables to store repository data.
*/
struct MockRepositoryInstance : public MockRepositoryStorage
{
    explicit MockRepositoryInstance(const std::string_view& options) 
        : _repoProperties(options) {}

private:
    std::string_view _repoProperties;
};
}  /// namespace MockObject
