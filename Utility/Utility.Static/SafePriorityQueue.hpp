#pragma once
#include <mutex>
#include <queue>

template <typename T, typename Comp = std::less<T>>
class SafePriorityQueue
{
private:
    std::mutex mGuardMutex, mUniqueMutex;
    std::condition_variable mBlock;

    std::priority_queue<T, std::vector<T>, Comp> mRawQueue;

public:
    SafePriorityQueue()                            = default;
    SafePriorityQueue(const SafePriorityQueue<T>&) = delete;

    ~SafePriorityQueue() { clear(); }

    const T& top()
    {
        std::lock_guard<std::mutex> guardLock(mGuardMutex);
        return mRawQueue.top();
    }

    T pop()
    {
        std::lock_guard<std::mutex> guardLock(mGuardMutex);
        T result = mRawQueue.top();
        mRawQueue.pop();
        return result;
    }

    void push(const T& item)
    {
        std::lock_guard<std::mutex> guardLock(mGuardMutex);
        mRawQueue.push(std::move(item));

        std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
        mBlock.notify_one();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> guardLock(mGuardMutex);
        return mRawQueue.empty();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> guardLock(mGuardMutex);
        return mRawQueue.size();
    }

    void clear()
    {
        std::lock_guard<std::mutex> guardLock(mGuardMutex);
        while (!mRawQueue.empty())
        {
            mRawQueue.pop();
        }
    }

    void wait()
    {
        while (empty())
        {
            std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
            mBlock.wait(uniqueLock);
        }
    }
};
