#include <any>
#include <future>
#include <functional>
#include <mutex>
#include <type_traits>
#include <queue>

#include "DataAccess/AbstarctRepositoryContainer.hpp"
#include "DataAccess/IServerRepositories.hpp"

#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"
#include "DataAccess.Postgre/PostgreRepositories.hpp"

namespace DataAccess
{
	class PostgreReposiotoryManager
	{
        public:

			template <typename TReturn, typename TIRepository, typename... Args>
			using MethodReference = std::function<TReturn(TIRepository::*)()>;

			using RequestTask = std::unique_ptr<std::packaged_task<std::any(void)>>;

			enum class EPriority : std::uint8_t
			{
				_1,  _2,  _3,  _4,  _5,
				_6,  _7,  _8,  _9,  _10,
				_11, _12, _13, _14, _15
			};

			struct RepositoryRequest
            {
			private:

                EPriority   mPriority;
				RequestTask mTask;

			public:
				RepositoryRequest(EPriority priority, RequestTask& task) 
					: mPriority(priority), mTask(std::move(task)) {}

				RepositoryRequest(RepositoryRequest&& request) = default;

			public:
				
				std::future<std::any>&& getFutureFromTask(void) noexcept
				{
					return mTask->get_future();
				}

			public:

                inline bool operator >  (const RepositoryRequest& task) { return this->mPriority >  task.mPriority; }
				inline bool operator <  (const RepositoryRequest& task) { return this->mPriority <  task.mPriority; }
				inline bool operator == (const RepositoryRequest& task) { return this->mPriority == task.mPriority; }
				inline bool operator != (const RepositoryRequest& task) { return this->mPriority != task.mPriority; }
            };

			template <typename TFromAny>
			struct FutureResult
			{
			private:

				std::future<std::any> mFuture;

			public:

				FutureResult(std::future<std::any>&& future)
					: mFuture(future) {}

				TFromAny&& get()
				{
					return std::move(std::any_cast<TFromAny>(mFuture.get()));
				}
			};

        private:

			std::unique_ptr<AbstarctRepositoryContainer> mRepositories;
            std::priority_queue<RepositoryRequest>		 mQueue;

			std::mutex mPushMutex;
			std::mutex mPopMutex;

        public:

			PostgreReposiotoryManager(std::shared_ptr<IAdapter> repositoryContainer)
				: mRepositories(std::make_unique<AbstarctRepositoryContainer>(repositoryContainer)), mQueue(), mPushMutex(), mPopMutex()
			{
				this->privateRegisterRepositories();
			}

		public:

			template <EPriority priority = EPriority::_15, typename TReturn, typename TIRepository, typename... Args>
            FutureResult<TReturn> pushRequest(const MethodReference<TReturn, TIRepository, Args...>& methodRef, Args... args) 
			{
				std::unique_lock<std::mutex> lck(mPushMutex);

                static_assert(std::is_base_of_v<IMasterRepository, TIRepository>								, "Current type is not implement IMasterRepository!");
                static_assert(std::is_polymorphic_v<TIRepository> && std::is_abstract_v<TIRepository>			, "Current type is not polymorphic or abstract!");
                static_assert(std::is_member_function_pointer_v<MethodReference<TIRepository, TReturn, Args...>>, "You passed not a method reference!");

				RepositoryRequest request = this->privateMakeRequest(methodRef, std::forward<Args>(args)...);
				FutureResult<TReturn> futureResult(request.getFutureFromTask());

				mQueue.push(std::move(request));

				return futureResult;
			}

			RepositoryRequest&& popRequest(void) noexcept
			{
				std::unique_lock<std::mutex> lck(mPopMutex);
				
				RepositoryRequest request = std::move(const_cast<RepositoryRequest&>(mQueue.top()));
				mQueue.pop();
				
				return std::move(request);
			}

		public:

			bool empty(void) const noexcept 
			{ 
				return mQueue.empty();
			}

		private:

			void privateRegisterRepositories()
			{
				mRepositories->registerRepository<IChannelsRepository, ChannelsRepository>();
				mRepositories->registerRepository<ILoginRepository   , LoginRepository   >();
				mRepositories->registerRepository<IMessagesRepository, MessagesRepository>();
				mRepositories->registerRepository<IRegisterRepository, RegisterRepository>();
				mRepositories->registerRepository<IRepliesRepository , RepliesRepository >();
			}

			template <EPriority priority, typename TReturn, typename TIRepository, typename... Args>
			RepositoryRequest&& privateMakeRequest(const MethodReference<TReturn, TIRepository, Args...>& methodRef, Args... args)
			{
				auto iRepository = mRepositories->getRepository<TIRepository>();

				RequestTask task = std::make_unique<RequestTask::pointer>
				(
					std::bind
					(
						[](TIRepository& iRepository, MethodReference<TReturn, TIRepository, Args...>& methodRef, Args&&... args)
						{
							return (iRepository.*methodRef)(args);
						},
						iRepository, methodRef, std::forward<Args>(args)...
					)
				);

				RepositoryRequest request(priority, task);

				return std::move(request);
			}
	};

	
}
