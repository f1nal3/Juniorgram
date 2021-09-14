#pragma once
#include "IAdapter.hpp"

namespace DataAccess
{
    /**
     * @brief The IMasterRepository base abstract class
     */
    struct IMasterRepository
    {
        std::shared_ptr<IAdapter> _adapter;

        /// Default virtual destructor
        virtual ~IMasterRepository() = default;
    };

}  // namespace DataAccess
