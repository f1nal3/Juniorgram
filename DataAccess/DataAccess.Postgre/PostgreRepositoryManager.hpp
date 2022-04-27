#pragma once
#include <any>
#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <type_traits>

#include <DataAccess/AbstractRepositoryContainer.hpp>
#include <DataAccess/IServerRepositories.hpp>

#include "PostgreRepositories.hpp"
#include "PostgreRepositoryContainer.hpp"

namespace DataAccess
{
template <typename TIRepository, typename TReturn, typename... TArgs>
using MethodReference = TReturn (TIRepository::*)(TArgs...);

using RequestTask = std::packaged_task<std::any(void)>;
using RawFuture   = std::future<std::any>;

/**
 * @brief  fmt function uses for converting arguments,
 *          that you pass to repository method to correct from.
 * @param  ref - argument that is passing to repository method.
 * @return Return correct format argument.
 *                 (For fundamental types like int, float, etc - by value, all other - by const ref).
 */
template <typename Type>
std::conditional_t<std::is_fundamental_v<Type>, std::remove_reference_t<Type>, const Type&> fmt(const Type& ref) { return ref; }

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
 * @brief   RepositoryRequest object uses for storing request in one container.
 *		    It is just a wrapper for std::packaged_task.
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
        this->_task     = std::move(other._task);
    }
    RepositoryRequest& operator=(RepositoryRequest&& other) noexcept
    {
        this->_priority = other._priority;
        this->_task     = std::move(other._task);

        return *this;
    }

public:
    /**
     * @brief  Getter for priority from request.
     * @return ePriority.
     */
    ePriority getPriority() const noexcept { return _priority; }

    /**
     * @brief  Getter for raw future from task.
     * @return future<any>.
     */
    RawFuture getFutureFromTask() { return _task.get_future(); }

public:
    bool operator>(const RepositoryRequest& task) const noexcept { return this->_priority > task._priority; }
    bool operator<(const RepositoryRequest& task) const noexcept { return this->_priority < task._priority; }
    bool operator==(const RepositoryRequest& task) const noexcept { return this->_priority == task._priority; }
    bool operator!=(const RepositoryRequest& task) const noexcept { return this->_priority != task._priority; }

    /**
     * @brief Invoke operator for request task.
     */
    inline void operator()() { _task(); }
};

/**
 * @brief   Custom future.
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
     * @brief   Like common std::future::get().
     * @details The difference is that returns not a raw data (any).
     * @return  Already known type value.
     */
    TFromAny get() { return std::any_cast<TFromAny>(_future.get()); }
};

/**
 * @class   PostgreRepositoryManager
 * @brief   PostgreRepositoryManager controls handler for repository requests.
 * @details Controls push to queue and further processing of requests (have own thread for it).
 */
class PostgreRepositoryManager
{
private:
    std::unique_ptr<AbstractRepositoryContainer> _repositories;
    std::priority_queue<RepositoryRequest>       _queue;

    std::mutex                                   _queueMutex;
    std::condition_variable                      _queueCV;

    std::atomic<bool>                            _handlerState;
    std::thread                                  _repositoryRequestsHandler;

public:
    explicit PostgreRepositoryManager(const std::shared_ptr<IAdapter>& repositoryContainer)
        : _repositories(std::make_unique<PostgreRepositoryContainer>(repositoryContainer)), _queue(), _queueMutex(), _handlerState(true)
    {
        this->privateRegisterRepositories();
    }

    ~PostgreRepositoryManager() { _repositoryRequestsHandler.join(); }

public:
    /**
     * @brief   Exists for add methods in the queue.
     *		    Pass all arguments (args) through 'fmt()' function!!!
     * @param   (Template arg) priority - priority of request.
     * @param   MethodReference - just a pointer to member function needed to add to the queue.
     * @param   args - list of args that passing to member function.
     * @return  Return future object from repository member function.
     */
    template <ePriority priority = ePriority::_15, typename TIRepository, typename TReturn, typename... TArgs>
    FutureResult<TReturn> pushRequest(const MethodReference<TIRepository, TReturn, TArgs...>& methodRef, TArgs&&... args)
    {
        static_assert(std::is_base_of_v<IMasterRepository, TIRepository>, "Current type is not implement IMasterRepository!");
        static_assert(std::is_polymorphic_v<TIRepository> && std::is_abstract_v<TIRepository>,
                      "Current type is not polymorphic or abstract!");
        static_assert(std::is_member_function_pointer_v<MethodReference<TIRepository, TReturn, TArgs...>>,
                      "You passed not a method reference!");

        std::unique_lock<std::mutex> lck(_queueMutex);

        RepositoryRequest     request = this->privateCreateRequest<priority>(methodRef, std::forward<TArgs>(args)...);
        FutureResult<TReturn> futureResult(request.getFutureFromTask());

        _queue.push(std::move(request));
        _queueCV.notify_one();
        return futureResult;
    }

    /**
     * @brief  Predicate for checking: is queue empty?
     * @return if empty - true, else - false.
     */
    bool empty() const noexcept { return _queue.empty(); }

    // \todo: Do better handler
    // (Probably, will be better to create thread pool
    // and use it for server and manager).
    /**
     * @brief   Requests handler.
     * @details Creates inside itself a thread that handle all requests in queue.
     */
    void handleRequests()
    {
        _repositoryRequestsHandler = std::thread([this]() {
            while (_handlerState)
            {
                auto request = this->privatePopRequest();
                if (request.has_value()) request.value()();
            }
        });
    }

    /**
     * @brief Stops handler.
     */
    void stopHandler()
    {
        _handlerState = false;
        _queueCV.notify_one();
    }

private:
    template <ePriority priority, typename TIRepository, typename TReturn, typename... TArgs>
    RepositoryRequest privateCreateRequest(const MethodReference<TIRepository, TReturn, TArgs...>& methodRef, TArgs&&... args)
    {
        auto iRepository = mRepositories->getRepository<TIRepository>();

        RequestTask task([iRepository, methodRef, args = std::make_tuple(std::forward<TArgs>(args)...)]() mutable -> std::any {
            return std::apply(methodRef, std::tuple_cat(std::make_tuple(iRepository), std::move(args)));
        });

        return RepositoryRequest(priority, task);
    }

    /**
     * @brief  Extract request from queue.
     * @return Repository request.
     */
    std::optional<RepositoryRequest> privatePopRequest() noexcept
    {
        std::unique_lock<std::mutex> lck(_queueMutex);

        _queueCV.wait(lck, [&]() { return !empty() || !_handlerState; });

        auto emp = empty();
        if (!_handlerState || emp) return {};

        RepositoryRequest request = std::move(const_cast<RepositoryRequest&>(_queue.top()));
        _queue.pop();

        return request;
    }

    /**
     * @brief Needs to register all repositories in repository container.
     */
    void privateRegisterRepositories()
    {
        _repositories->registerRepository<IChannelsRepository, ChannelsRepository>();
        _repositories->registerRepository<ILoginRepository, LoginRepository>();
        _repositories->registerRepository<IMessagesRepository, MessagesRepository>();
        _repositories->registerRepository<IRegisterRepository, RegisterRepository>();
        _repositories->registerRepository<IRepliesRepository, RepliesRepository>();
        _repositories->registerRepository<IDirectMessageRepository, DirectMessageRepository>();
    }
};

}  /// namespace DataAccess
