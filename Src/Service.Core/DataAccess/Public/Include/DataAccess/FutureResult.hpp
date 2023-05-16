#pragma once

#include <any>
#include <future>

namespace DataAccess
{
    using RawFuture = std::future<std::any>;

    /**
     * @brief Custom future.
     * @details Uses like common wrapper for std::future.
     */
    template <typename TFromAny>
    struct FutureResult
    {
    private:
        RawFuture _future;

    public:
        explicit FutureResult(RawFuture&& future) : _future(std::move(future)) {}

        /**
         * @brief Like common std::future::get().
         * @details The difference is that returns not a raw data (any).
         * @return Already known type value.
         */
        TFromAny get() {
            return std::any_cast<TFromAny>(_future.get());
        }
    };
    
} // namespace DataAccess
