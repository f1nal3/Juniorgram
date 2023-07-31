#pragma once

#include <any>
#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <type_traits>
#include <tuple>

#include "AbstractRepositoryContainer.hpp"
#include "RepositoryRequest.hpp"

namespace DataAccess
{
template <typename TIRepository, typename TReturn, typename... TArgs>
using MethodReference = TReturn (TIRepository::*)(TArgs...);

/**
 * @brief fmt function uses for converting arguments, that you pass to repository method to correct form.
 * @param ref argument that is passing to repository method.
 * @return Return correct format argument: for fundamental types like int, float, etc - by value, all other - by const ref.
 */
template <typename Type>
std::conditional_t<std::is_fundamental_v<Type>, std::remove_reference_t<Type>, const Type&> fmt(const Type& ref)
{
    return ref;
}

/**
 * @class IRepositoryManager
 * @brief IRepositoryManager is template class, which realization controls handler for repository requests.
 * @details Class controls pushing to queue and further processing of requests (have own thread for it).
 * Requests with the highest priority are at the top of the queue.
 */
class IRepositoryManager
{
public:
    IRepositoryManager() = default;

    void init(std::unique_ptr<AbstractRepositoryContainer> repositories) { _repositories = std::move(repositories); }

    /**
     * @brief Destroy the IRepositoryManager object but before synchronize all threads.
     */
    virtual ~IRepositoryManager() { _repositoryRequestsHandler.join(); }

    IRepositoryManager(const IRepositoryManager&)            = delete;
    IRepositoryManager(IRepositoryManager&&)                 = delete;
    IRepositoryManager& operator=(const IRepositoryManager&) = delete;
    IRepositoryManager& operator=(IRepositoryManager&&)      = delete;

    /**
     * @brief Exists for add methods in the queue.
     * @warning Pass all arguments (args) through 'fmt()' function
     * @param (Template arg) priority - priority of request. By default, this is equal to the maximum priority.
     * @param MethodReference - just a pointer to member function needed to add to the queue.
     * @param args - list of args that passing to member function.
     * @return Return future object from repository member function.
     */
    template <ePriority priority = ePriority::_15, typename TIRepository, typename TReturn, typename... TArgs>
    FutureResult<TReturn> pushRequest(const MethodReference<TIRepository, TReturn, TArgs...>& methodRef, TArgs&&... args)
    {
        static_assert(std::is_base_of_v<IMasterRepository, TIRepository>, "Current type is not implement IMasterRepository!");
        static_assert(std::is_polymorphic_v<TIRepository> && std::is_abstract_v<TIRepository>,
                      "Current type is not polymorphic or abstract!");
        static_assert(std::is_member_function_pointer_v<MethodReference<TIRepository, TReturn, TArgs...>>,
                      "You passed not a method reference!");

        std::unique_lock lock(_queueMutex);

        RepositoryRequest     request = this->createRequest<priority>(methodRef, std::forward<TArgs>(args)...);
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
    // (Pay attention, lack of performance.
    // Consider using a thread pool).
    /**
     * @brief   Requests handler.
     * @details Creates inside itself a thread that handle all requests in queue.
     */
    void handleRequests()
    {
        _repositoryRequestsHandler = std::thread([this]() {
            while (_handlerState)
            {
                auto request = this->popRequest();
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
    RepositoryRequest createRequest(const MethodReference<TIRepository, TReturn, TArgs...>& methodRef, TArgs&&... args)
    {
        auto iRepository = _repositories->getRepository<TIRepository>();

        RequestTask task([iRepository, methodRef, args = std::make_tuple(std::forward<TArgs>(args)...)]() mutable -> std::any {
            return std::apply(methodRef, std::tuple_cat(std::make_tuple(iRepository), std::move(args)));
        });

        return RepositoryRequest(priority, task);
    }

    /**
     * @brief  Extract request from queue.
     * @return Repository request.
     */
    std::optional<RepositoryRequest> popRequest() noexcept
    {
        std::unique_lock lock(_queueMutex);

        _queueCV.wait(lock, [this]() { return !empty() || !_handlerState; });

        if (!_handlerState || empty()) return {};

        RepositoryRequest request = std::move(const_cast<RepositoryRequest&>(_queue.top()));
        _queue.pop();

        return request;
    }

private:
    std::unique_ptr<AbstractRepositoryContainer> _repositories;
    std::priority_queue<RepositoryRequest>       _queue;

    std::mutex              _queueMutex;
    std::condition_variable _queueCV;

    std::atomic<bool> _handlerState = true;
    std::thread       _repositoryRequestsHandler;
};
}  // namespace DataAccess
