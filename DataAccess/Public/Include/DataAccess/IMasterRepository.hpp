#pragma once

#include <iostream>
#include <memory>

namespace DataAccess
{

/**
 * @brief The IMasterRepository base abstract class
 */
struct IMasterRepository
{
    /**
    * @brief virtual destructor.
    */
    virtual ~IMasterRepository() = default;
};
}  // namespace DataAccess
