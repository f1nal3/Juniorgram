#pragma once

#include <vector>

#include "IMasterRepository.hpp"

namespace DataAccess
{
struct MessageInfo;

class IMessageRepository : public IMasterRepository
{
    virtual void addMessages(std::vector<MessageInfo>& messages) = 0;
};
}  // namespace DataAccess
