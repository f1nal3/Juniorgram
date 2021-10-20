#pragma once
#include <DataAccess/AbstractRepositoryContainer.hpp>
#include <DataAccess/IServerRepositories.hpp>
#include <any>
#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <type_traits>

#include "PostgreRepositories.hpp"
#include "PostgreRepositoryContainer.hpp"

namespace DataAccess
{
template <typename TIRepository, typename TReturn, typename... TArgs>
using MethodReference = TReturn (TIRepository::*)(TArgs...);

using RequestTask = std::packaged_task<std::any(void)>;
using RawFuture   = std::future<std::any>;

/* @brief  fmt function uses for converting arguments,
*          that you pass to repository method to correct from.
*  @param  ref - argument that is passing to reposiroty method.
*  @return Return correct format argument.
                   (For fundamental types like int, float, etc - by value, all other - by const ref).
*/
template <typename Type>
std::conditional_t<std::is_fundamental_v<Type>, std::remove_reference_t<Type>, const Type&> fmt(const Type& ref)
{
    return ref;
}

/* @brief   Priority enum.
 *  @details The more priority value the higher priority.
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

/* @brief   RepositoryRequest object uses for storing request in one container.
 *		    It is just a wrapper for std::packaged_task.
 *  @details It is also stores a priority of this request.
 */
struct RepositoryRequest
{
private:
    ePriority   mPriority;
    RequestTask mTask;

public:
    RepositoryRequest(ePriority priority, RequestTask& task) : mPriority(priority), mTask(std::move(task)) {}

    RepositoryRequest(RepositoryRequest&& other) noexcept
    {
        this->mPriority = other.mPriority;
        this->mTask     = std::move(other.mTask);
    }
    RepositoryRequest& operator=(RepositoryRequest&& other) noexcept
    {
        this->mPriority = other.mPriority;
        this->mTask     = std::move(other.mTask);

        return *this;
    }

public:
    /* @brief  Getter for priority from request.
     *  @return ePriority.
     */
    ePriority getPriority(void) const noexcept { return mPriority; }

    /* @brief  Getter for raw future from task.
     *  @return future<any>.
     */
    RawFuture getFutureFromTask(void) { return mTask.get_future(); }

public:
    bool operator>(const RepositoryRequest& task) const noexcept { return this->mPriority > task.mPriority; }
    bool operator<(const RepositoryRequest& task) const noexcept { return this->mPriority < task.mPriority; }
    bool operator==(const RepositoryRequest& task) const noexcept { return this->mPriority == task.mPriority; }
    bool operator!=(const RepositoryRequest& task) const noexcept { return this->mPriority != task.mPriority; }

    /* @brief Invoke operator for request task.
     */
    inline void operator()(void) { mTask(); }
};

/* @brief   Custom future.
 *  @details Uses like common wrapper for std::future.
 */
template <typename TFromAny>
struct FutureResult
{
private:
    RawFuture mFuture;

public:
    FutureResult(RawFuture&& future) : mFuture(std::move(future)) {}

    /* @brief   Like common std::future::get().
     *  @details The difference is that returns not a raw data (any).
     *  @return  Already known type value.
     */
    TFromAny get(void) { return std::any_cast<TFromAny>(mFuture.get()); }
};

/* @brief   PostgreRepositoryManager controls handler for repository requests.
 *  @details Controls push to queue and furthur processing of requests (have own thread for it).
 */
class PostgreReposiotoryManager
{
private:
    std::unique_ptr<AbstractRepositoryContainer> mRepositories;
    std::priority_queue<RepositoryRequest>       mQueue;

    std::mutex mPushMutex;
    std::mutex mPopMutex;

    std::atomic<bool> mHandlerState;
    std::thread       mReposritoryRequestsHandler;

public:
    PostgreReposiotoryManager(std::shared_ptr<IAdapter> repositoryContainer)
        : mRepositories(std::make_unique<PostgreRepositoryContainer>(repositoryContainer)),
          mQueue(),
          mPushMutex(),
          mPopMutex(),
          mHandlerState(true)
    {
        this->privateRegisterRepositories();
    }

    ~PostgreReposiotoryManager() { mReposritoryRequestsHandler.join(); }

public:
    /* @brief   Exists for add methods in the queue.
     *		    Pass all arguments (args) through 'fmt()' function!!!
     *  @param   (Template arg) priority - priority of request.
     *  @param   MethodReference - just a pointer to member function needed to add to the queue.
     *  @param   args - list of args that passing to member function.
     *  @return  Return future object from repository member function.
     */
    template <ePriority priority = ePriority::_15, typename TIRepository, typename TReturn, typename... TArgs>
    FutureResult<TReturn> pushRequest(const MethodReference<TIRepository, TReturn, TArgs...>& methodRef, TArgs&&... args)
    {
        std::unique_lock<std::mutex> lck(mPushMutex);

        static_assert(std::is_base_of_v<IMasterRepository, TIRepository>, "Current type is not implement IMasterRepository!");
        static_assert(std::is_polymorphic_v<TIRepository> && std::is_abstract_v<TIRepository>,
                      "Current type is not polymorphic or abstract!");
        static_assert(std::is_member_function_pointer_v<MethodReference<TIRepository, TReturn, TArgs...>>,
                      "You passed not a method reference!");

        RepositoryRequest     request = this->privateCreateRequest<priority>(methodRef, std::forward<TArgs>(args)...);
        FutureResult<TReturn> futureResult(request.getFutureFromTask());

        mQueue.push(std::move(request));

        return futureResult;
    }

    /* @brief  Predicat for checking: is queue empty?
     *  @return if empty - true, else - false.
     */
    bool empty(void) const noexcept { return mQueue.empty(); }

    // TODO: Do better handler
    // (Probably, will be better to create thread pool
    // and use it for server and manager).
    /* @brief   Requests handler.
     *  @details Creates inside itself a thread that handle all requsts in queue.
     */
    void handleRequests(void)
    {
        mReposritoryRequestsHandler = std::thread([this]() {
            while (mHandlerState)
            {
                if (!this->empty())
                {
                    this->privatePopRequest()();
                }
            }
        });
    }

    /* @brief Stopping handler.
     */
    void stopHandler() { mHandlerState = false; }

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

    /* @brief  Extract requst from queue.
     *  @return Repository request.
     */
    RepositoryRequest privatePopRequest(void) noexcept
    {
        std::unique_lock<std::mutex> lck(mPopMutex);

        RepositoryRequest request = std::move(const_cast<RepositoryRequest&>(mQueue.top()));
        mQueue.pop();

        return request;
    }

    /* @brief Needs to register all repositories in repository container.
     */
    void privateRegisterRepositories(void)
    {
        mRepositories->registerRepository<IChannelsRepository, ChannelsRepository>();
        mRepositories->registerRepository<ILoginRepository, LoginRepository>();
        mRepositories->registerRepository<IMessagesRepository, MessagesRepository>();
        mRepositories->registerRepository<IRegisterRepository, RegisterRepository>();
        mRepositories->registerRepository<IRepliesRepository, RepliesRepository>();
    }
};

}  // namespace DataAccess
