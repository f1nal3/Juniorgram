#pragma once

#include "common.h"

namespace network
{
	template<typename T>
	class SafeQueue
	{
		std::mutex mScopedMutex, mUniqueMutex;
		std::condition_variable mBlock;

		std::deque<T> mRawQueue;

	public:
		SafeQueue() = default;
		SafeQueue(const SafeQueue<T>& other) = delete;

		~SafeQueue()
		{ 
			clear();
		}

		const T& front()
		{
			std::scoped_lock scopedLock(mScopedMutex);
			return mRawQueue.front();
		}

		const T& back()
		{
			std::scoped_lock scopedLock(mScopedMutex);
			return mRawQueue.back();
		}

		T pop_front()
		{
			std::scoped_lock scopedLock(mScopedMutex);
			T result = std::move(mRawQueue.front());
            mRawQueue.pop_front();
            return result;
        }
        
        T pop_back() {
            std::scoped_lock scopedLock(mScopedMutex);
            T result = std::move(mRawQueue.back());
            mRawQueue.pop_back();
            return result;
        }
        
        void push_back(const T &item) {
            std::scoped_lock scopedLock(mScopedMutex);
            mRawQueue.emplace_back(std::move(item));
            
            std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
            mBlock.notify_one();
        }
        
        void push_front(const T &item) {
            std::scoped_lock scopedLock(mScopedMutex);
            mRawQueue.emplace_front(std::move(item));
            
            std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
            mBlock.notify_one();
        }
        
        bool empty() {
            std::scoped_lock scopedLock(mScopedMutex);
            return mRawQueue.empty();
        }
        
        size_t size() {
            std::scoped_lock scopedLock(mScopedMutex);
            return mRawQueue.size();
        }
        
        void clear() {
            std::scoped_lock scopedLock(mScopedMutex);
            mRawQueue.clear();
        }
        
        void wait() {
            while (empty()) {
                std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
                mBlock.wait(uniqueLock);
            }
        }
    };
}
