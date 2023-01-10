#pragma once

#include "DataAccess/AbstractRepositoryContainer.hpp"
#include "DataAccess/RepositoryRequest.hpp"

#include "MockRepositories.hpp"

namespace MockDataAccess
{
template <typename TIRepository, typename TReturn, typename... TArgs>
using MethodReference = TReturn (TIRepository::*)(TArgs...);

template <typename Type>
std::conditional_t<std::is_fundamental_v<Type>, std::remove_reference_t<Type>, 
    const Type&> fmt(const Type& ref)
{
    return ref;
}

class MockIRepositoryManager
{
private:
    std::unique_ptr<DataAccess::AbstractRepositoryContainer> _repositories;
    std::priority_queue<DataAccess::RepositoryRequest>       _queue;

    std::mutex                                               _queueMutex;
    std::condition_variable                                  _queueCV;

    std::atomic<bool>                                        _handlerState = true;
    std::thread                                              _repositoryRequestsHandler;

public:
    MockIRepositoryManager() = default;

    explicit MockIRepositoryManager(const std::shared_ptr<DataAccess::IAdapter>& repositoryContainer)
        : _repositories(std::make_unique<DataAccess::AbstractRepositoryContainer>(repositoryContainer)) 
    {
        this->privateRegisterTestRepositories();
    }

    virtual ~MockIRepositoryManager() 
    {
        _repositoryRequestsHandler.join();
    }

    template <DataAccess::ePriority priority = DataAccess::ePriority::_15, typename TIRepository, typename TReturn, typename... TArgs>
    DataAccess::FutureResult<TReturn> pushRequest(const MethodReference<TIRepository, TReturn, TArgs...>& methodRef, TArgs&&... args)
    {
        static_assert(std::is_base_of_v<DataAccess::IMasterRepository, TIRepository>, "Current type is not implement IMasterRepository!");
        static_assert(std::is_polymorphic_v<TIRepository> && std::is_abstract_v<TIRepository>,
                      "Current type is not polymorphic or abstract!");
        static_assert(std::is_member_function_pointer_v<MethodReference<TIRepository, TReturn, TArgs...>>,
                      "You passed not a method reference!");

        std::unique_lock lock(_queueMutex);

        DataAccess::RepositoryRequest request = this->privateCreateRequest<priority>
            (methodRef, std::forward<TArgs>(args)...);
        DataAccess::FutureResult<TReturn> futureResult(request.getFutureFromTask());

        _queue.push(std::move(request));
        _queueCV.notify_one();
        return futureResult;
    }

    bool empty() const noexcept { return _queue.empty(); }

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

    void stopHandler()
    {
        _handlerState = false;
        _queueCV.notify_one();
    }

private:
    template <DataAccess::ePriority priority, typename TIRepository, typename TReturn, typename... TArgs>
    DataAccess::RepositoryRequest privateCreateRequest(const MethodReference<TIRepository, TReturn, TArgs...>& methodRef, TArgs&&... args)
    {
        auto iRepository = _repositories->getRepository<TIRepository>();

        DataAccess::RequestTask task([iRepository, methodRef, args = std::make_tuple(std::forward<TArgs>(args)...)]() mutable -> std::any {
            return std::apply(methodRef, std::tuple_cat(std::make_tuple(iRepository), std::move(args)));
        });

        return DataAccess::RepositoryRequest(priority, task);
    }

    std::optional<DataAccess::RepositoryRequest> privatePopRequest() noexcept
    {
        std::unique_lock lock(_queueMutex);

        _queueCV.wait(lock, [this]() { return !empty() || !_handlerState; });

        if (!_handlerState || empty()) return {};

        DataAccess::RepositoryRequest result(std::move
            (const_cast<DataAccess::RepositoryRequest&>(_queue.top())));
        _queue.pop();

        return result;
    }

    void privateRegisterTestRepositories() 
    {
        _repositories->registerRepository<DataAccess::IChannelsRepository, MockRepositories::testChannelsRepository>();
        _repositories->registerRepository<DataAccess::ILoginRepository, MockRepositories::testLoginRepository>();
        _repositories->registerRepository<DataAccess::IMessagesRepository, MockRepositories::testMessagesRepository>();
        _repositories->registerRepository<DataAccess::IRegisterRepository, MockRepositories::testRegisterRepository>();
        _repositories->registerRepository<DataAccess::IRepliesRepository, MockRepositories::testRepliesRepository>();
        _repositories->registerRepository<DataAccess::IDirectMessageRepository, MockRepositories::testDirectMessageRepository>();
    }
};
} // namespace MockDataAccess
