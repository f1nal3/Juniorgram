#pragma once

#include <iostream>
#include <memory>

namespace DataAccess
{

/**
*  @class IMasterRepository
 * @brief The IMasterRepository base abstract class
 * @details Class used as a base of interfaces for repositories on client
 * and server sides.
 */
struct IMasterRepository
{
    /**
    * @brief virtual destructor.
    */
    virtual ~IMasterRepository() = default;
};
}  // namespace DataAccess
