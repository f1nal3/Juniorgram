#pragma once
#include <any>
#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <type_traits>

#include "AbstractRepositoryContainer.hpp"
#include "IServerRepositories.hpp"
#include "RepositoryRequest.hpp"

#include <DataAccess.Postgre/PostgreRepositories.hpp>

namespace DataAccess
{
    template <typename TIRepository, typename TReturn, typename... TArgs>
    using MethodReference = TReturn(TIRepository::*)(TArgs...);

    /**
    * @brief  fmt function uses for converting arguments,
    *          that you pass to repository method to correct from.
    * @param  ref - argument that is passing to repository method.
    * @return Return correct format argument.
    *                 (For fundamental types like int, float, etc - by value, all other - by const ref).
    */
    template <typename Type>
    std::conditional_t<std::is_fundamental_v<Type>, std::remove_reference_t<Type>, const Type&> fmt(const Type& ref) {return ref;}

    /**
    * @class   IRepositoryManager
    * @brief   IRepositoryManager is template class, which realization controls handler for repository requests.
    * @details Controls push to queue and further processing of requests (have own thread for it).
    */
    template <typename TRepositoryContainer>
    class IRepositoryManager
    {
    private:
        std::unique_ptr<TRepositoryContainer>        _repositories;
        std::priority_queue<RepositoryRequest>       _queue;

        std::mutex                                   _queueMutex;
        std::condition_variable                      _queueCV;

        std::atomic<bool>                            _handlerState;
        std::thread                                  _repositoryRequestsHandler;

    public:
        explicit IRepositoryManager(const std::shared_ptr<IAdapter>& repositoryContainer)
            :_repositories(std::make_unique<TRepositoryContainer>(repositoryContainer)),
            _queue(),
            _queueMutex(),
            _handlerState(true)
        {
            this->privateRegisterRepositories();
        }

        ~IRepositoryManager() {
            _repositoryRequestsHandler.join();
        }

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

            std::unique_lock<std::mutex> lock(_queueMutex);

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
        bool empty() const noexcept {
            return _queue.empty();
        }

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
                while ( _handlerState )
                {
                    auto request = this->privatePopRequest();
                    if ( request.has_value() ) request.value()();
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
            auto iRepository = _repositories->getRepository<TIRepository>();

            RequestTask task([iRepository, methodRef, args = std::make_tuple(std::forward<TArgs>(args)...)]() mutable->std::any {
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
            std::unique_lock<std::mutex> lock(_queueMutex);

            _queueCV.wait(lock, [&]() { return !empty() || !_handlerState; });

            if ( !_handlerState || empty() ) return {};

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
}
