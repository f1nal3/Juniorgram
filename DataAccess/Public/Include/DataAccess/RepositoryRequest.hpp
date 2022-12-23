#pragma once
#include "FutureResult.hpp"

namespace DataAccess
{
    using RequestTask = std::packaged_task<std::any(void)>;

    /**
     * @brief   Priority enum.
     * @details The more priority value the higher priority.
     */
    enum class ePriority : std::uint8_t
    {
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        _10,
        _11,
        _12,
        _13,
        _14,
        _15
    };

    /**
     * @brief RepositoryRequest object uses for storing request in one container.
     * It is just a wrapper for std::packaged_task.
     * @details It is also stores a priority of this request.
     */
    struct RepositoryRequest
    {
    private:
        ePriority   _priority;
        RequestTask _task;

    public:
        explicit RepositoryRequest(ePriority priority, RequestTask& task) : _priority(priority), _task(std::move(task)) {}

        RepositoryRequest(RepositoryRequest&& other) noexcept
        {
            this->_priority = other._priority;
            this->_task = std::move(other._task);
        }
        RepositoryRequest& operator=(RepositoryRequest&& other) noexcept
        {
            this->_priority = other._priority;
            this->_task = std::move(other._task);

            return *this;
        }

    public:
        /**
         * @brief  Getter for priority from request.
         * @return ePriority.
         */
        ePriority getPriority() const noexcept {
            return _priority;
        }

        /**
        * @brief  Getter for raw future from task.
        * @return future<any>.
        */
        RawFuture getFutureFromTask() {
            return _task.get_future();
        }

    public:
        bool operator>(const RepositoryRequest& task) const noexcept {
            return this->_priority > task._priority;
        }
        bool operator<(const RepositoryRequest& task) const noexcept {
            return this->_priority < task._priority;
        }
        bool operator==(const RepositoryRequest& task) const noexcept {
            return this->_priority == task._priority;
        }
        bool operator!=(const RepositoryRequest& task) const noexcept {
            return this->_priority != task._priority;
        }

    /**
     * @brief Invoke operator for request task.
     */
        inline void operator()() {
            _task();
        }
    };

} /// namespace DataAccesss
