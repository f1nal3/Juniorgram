#pragma once 

#include "MockRepositoryStorage.hpp"

namespace MockObject
{
struct MockRepositoryInstance : public MockRepositoryStorage
{
    explicit MockRepositoryInstance(const std::string_view& options) : _repoProperties(options) {}

private:
    std::string_view _repoProperties;
};
}  /// namespace MockObject
