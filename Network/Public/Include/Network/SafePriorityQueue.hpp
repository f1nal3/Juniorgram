#pragma once
#include <queue>

template <typename T, typename Comp = std::less<>>
class SafePriorityQueue
{
private:
    std::mutex mScopedMutex, mUniqueMutex;
    std::condition_variable mBlock;

    std::priority_queue<T, std::vector<T>, Comp> mRawQueue;

public:
    SafePriorityQueue()                      = default;
    SafePriorityQueue(const SafeQueue<T>& other) = delete;

    ~SafePriorityQueue() { clear(); }

    const T& top()
    {
        std::scoped_lock scopedLock(mScopedMutex);
        return mRawQueue.top();
    }

    T pop()
    {
        std::scoped_lock scopedLock(mScopedMutex);
        T result = std::move(mRawQueue.pop());
        mRawQueue.pop();
        return result;
    }

    void push(const T& item)
    {
        std::scoped_lock scopedLock(mScopedMutex);
        mRawQueue.push(std::move(item));

        std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
        mBlock.notify_one();
    }

    bool empty()
    {
        std::scoped_lock scopedLock(mScopedMutex);
        return mRawQueue.empty();
    }

    size_t size()
    {
        std::scoped_lock scopedLock(mScopedMutex);
        return mRawQueue.size();
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