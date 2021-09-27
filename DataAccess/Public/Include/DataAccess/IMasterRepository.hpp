#pragma once
#include <memory>
#include <iostream>

namespace DataAccess
{
    /**
     * @brief The IMasterRepository base abstract class
     */
    struct IMasterRepository
    {
        /// Default virtual destructor
        virtual ~IMasterRepository() = default;
    };

}  // namespace DataAccess
