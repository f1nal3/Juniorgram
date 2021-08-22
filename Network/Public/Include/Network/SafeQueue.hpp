#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

namespace Network
{
template <typename T>
/**
 * @brief The tamplate SafeQueue class
 */
class SafeQueue
{
    std::mutex              mScopedMutex, mUniqueMutex;
    std::condition_variable mBlock;

    std::deque<T> mRawQueue;

public:
    // Default SafeQueue constructor
    SafeQueue()                          = default;

    // SafeQueue copy constructor delete
    SafeQueue(const SafeQueue<T>& other) = delete;

    // SafeQueue destructor for clear method
    ~SafeQueue() { clear(); }

    // Method for access to front value
    const T& front()
    {
        std::scoped_lock scopedLock(mScopedMutex);
        return mRawQueue.front();
    }

    // Method for access to back value
    const T& back()
    {
        std::scoped_lock scopedLock(mScopedMutex);
        return mRawQueue.back();
    }

    // Method to remove front value
    T pop_front()
    {
        std::scoped_lock scopedLock(mScopedMutex);
        T                result = std::move(mRawQueue.front());
        mRawQueue.pop_front();
        return result;
    }

    // Method to remove back value
    T pop_back()
    {
        std::scoped_lock scopedLock(mScopedMutex);
        T                result = std::move(mRawQueue.back());
        mRawQueue.pop_back();
        return result;
    }

    // Method to add back value
    void push_back(const T& item)
    {
        std::scoped_lock scopedLock(mScopedMutex);
        mRawQueue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
        mBlock.notify_one();
    }

    // Method to add front value
    void push_front(const T& item)
    {
        std::scoped_lock scopedLock(mScopedMutex);
        mRawQueue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
        mBlock.notify_one();
    }

    // Method for checking if a SafeQueue is empty
    bool empty()
    {
        std::scoped_lock scopedLock(mScopedMutex);
        return mRawQueue.empty();
    }

    // Method for return size
    size_t size()
    {
        std::scoped_lock scopedLock(mScopedMutex);
        return mRawQueue.size();
    }

    // Method for clear SafeQueue
    void clear()
    {
        std::scoped_lock scopedLock(mScopedMutex);
        mRawQueue.clear();
    }

    // Method to wait which uses condition variable
    void wait()
    {
        while (empty())
        {
            std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
            mBlock.wait(uniqueLock);
        }
    }
};
}  // namespace Network
