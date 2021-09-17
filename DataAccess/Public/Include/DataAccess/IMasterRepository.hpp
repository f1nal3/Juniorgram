#pragma once
#include <memory>
#include <iostream>

#include "IAdapter.hpp"

namespace DataAccess
{
    /**
     * @brief The IMasterRepository base abstract class
     */
    struct IMasterRepository
    {
    protected:

        std::shared_ptr<IAdapter> _adapter;

    public:

        /// Default virtual destructor
        virtual ~IMasterRepository() = default;
    };

}  // namespace DataAccess
