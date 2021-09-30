#pragma once
#include <any>
#include <future>
#include <functional>
#include <mutex>
#include <type_traits>
#include <queue>

#include "DataAccess/AbstarctRepositoryContainer.hpp"
#include "DataAccess/IServerRepositories.hpp"

#include "PostgreRepositoryContainer.hpp"
#include "PostgreRepositories.hpp"

namespace DataAccess
{
	template <typename Type>
	std::conditional_t<std::is_fundamental_v<Type>, std::remove_reference_t<Type>, const Type&> byRef(const Type& ref)
	{
		return ref;
	}

	enum class EPriority : std::uint8_t
	{
		_1, _2, _3, _4, _5,
		_6, _7, _8, _9, _10,
		_11, _12, _13, _14, _15
	};

	template <typename TIRepository, typename TReturn, typename... TArgs>
	using MethodReference = TReturn(TIRepository::*)(TArgs...);

	using RequestTask = std::packaged_task<std::any(void)>;

	struct RepositoryRequest
	{
	private:

		EPriority   mPriority;
		RequestTask mTask;

	public:
		RepositoryRequest(EPriority priority, RequestTask& task)
			: mPriority(priority), mTask(std::move(task)) {}

		RepositoryRequest(RepositoryRequest&& other)
		{
			this->mPriority = other.mPriority;
			this->mTask = std::move(other.mTask);
		}
		RepositoryRequest& operator = (RepositoryRequest&& other)
		{
			this->mPriority = other.mPriority;
			this->mTask = std::move(other.mTask);

			return *this;
		}

	public:

		EPriority getPriority(void) const noexcept
		{
			return this->mPriority;
		}

		std::future<std::any>&& getFutureFromTask(void) noexcept
		{
			return std::move(mTask.get_future());
		}

	public:

		bool operator >  (const RepositoryRequest& task) const noexcept { return this->mPriority > task.mPriority; }
		bool operator <  (const RepositoryRequest& task) const noexcept { return this->mPriority < task.mPriority; }
		bool operator == (const RepositoryRequest& task) const noexcept { return this->mPriority == task.mPriority; }
		bool operator != (const RepositoryRequest& task) const noexcept { return this->mPriority != task.mPriority; }
	};

	template <typename TFromAny>
	struct FutureResult
	{
	private:

		std::future<std::any> mFuture;

	public:

		FutureResult(std::future<std::any>&& future)
			: mFuture(std::move(future)) {}

		TFromAny&& get()
		{
			return std::move(std::any_cast<TFromAny>(mFuture.get()));
		}
	};

	class PostgreReposiotoryManager
	{
        private:

			std::unique_ptr<AbstarctRepositoryContainer> mRepositories;
            std::priority_queue<RepositoryRequest>       mQueue;

			std::mutex mPushMutex;
			std::mutex mPopMutex;

        public:

			PostgreReposiotoryManager(std::shared_ptr<IAdapter> repositoryContainer)
				: mRepositories(std::make_unique<PostgreRepositoryContainer>(repositoryContainer)), mQueue(), mPushMutex(), mPopMutex()
			{
				this->privateRegisterRepositories();
			}

		public:

			template <EPriority priority = EPriority::_15, typename TIRepository, typename TReturn, typename... TArgs>
            FutureResult<TReturn> pushRequest(const MethodReference<TIRepository, TReturn, TArgs...>& methodRef, TArgs&&... args)
			{
				std::unique_lock<std::mutex> lck(mPushMutex);

                static_assert(std::is_base_of_v<IMasterRepository, TIRepository>								, "Current type is not implement IMasterRepository!");
                static_assert(std::is_polymorphic_v<TIRepository> && std::is_abstract_v<TIRepository>			, "Current type is not polymorphic or abstract!");
                static_assert(std::is_member_function_pointer_v<MethodReference<TIRepository, TReturn, TArgs...>>, "You passed not a method reference!");

				RepositoryRequest request = this->privateCreateRequest<priority>(methodRef, std::forward<TArgs>(args)...);
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

			template <EPriority priority, typename TIRepository, typename TReturn, typename... TArgs>
			RepositoryRequest&& privateCreateRequest(const MethodReference<TIRepository, TReturn, TArgs...>& methodRef, TArgs&&... args)
			{
				auto iRepository = mRepositories->getRepository<TIRepository>();

				RequestTask task(
					[&iRepository, &methodRef, args = std::make_tuple(std::forward<TArgs>(args)...)]() mutable -> std::any
					{
						return std::apply(methodRef, std::tuple_cat(std::make_tuple(&*iRepository), std::move(args)));
					}
				);

				RepositoryRequest request(EPriority::_15, task);
				return std::move(request);
			}
	};

	
}
