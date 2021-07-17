#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

namespace Network
{
template <typename T>
class SafeQueue
{
    std::mutex mScopedMutex, mUniqueMutex;
    std::condition_variable mBlock;

    std::deque<T> mRawQueue;

public:
    SafeQueue()                          = default;
    SafeQueue(const SafeQueue<T>& other) = delete;

    ~SafeQueue();

    const T& front();
    
    const T& back();

    T pop_front();

    T pop_back();

    void push_back(const T& item);

    void push_front(const T& item);

    bool empty();

    size_t size();

    void clear();

    void wait();

};
}  // namespace Network
