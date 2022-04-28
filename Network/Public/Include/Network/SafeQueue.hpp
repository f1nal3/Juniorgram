#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

namespace Network
{
template <typename TQueue>
/**
 * @brief The tamplate SafeQueue class.
 */
class SafeQueue
{
    std::mutex              _scopedMutex, _uniqueMutex;
    std::condition_variable _block;

    std::deque<TQueue> _rawQueue;

public:
    /// Default SafeQueue constructor.
    SafeQueue()                          = default;

    /// SafeQueue copy constructor delete.
    SafeQueue(const SafeQueue<TQueue>& other) = delete;

    /// SafeQueue destructor for clear method.
    ~SafeQueue() { clear(); }

    /// Method for access to front value.
    const TQueue& front()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        return _rawQueue.front();
    }

    /// Method for access to back value.
    const TQueue& back()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        return _rawQueue.back();
    }

    /// Method to remove front value.
    TQueue pop_front()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        TQueue           result = std::move(_rawQueue.front());
        _rawQueue.pop_front();
        return result;
    }

    /// Method to remove back value.
    TQueue pop_back()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        TQueue           result = std::move(_rawQueue.back());
        _rawQueue.pop_back();
        return result;
    }

    /// Method to add back value.
    void push_back(const TQueue& item)
    {
        std::scoped_lock scopedLock(_scopedMutex);
        _rawQueue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> uniqueLock(_uniqueMutex);
        _block.notify_one();
    }

    /// Method to add front value.
    void push_front(const TQueue& item)
    {
        std::scoped_lock scopedLock(_scopedMutex);
        _rawQueue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> uniqueLock(_uniqueMutex);
        _block.notify_one();
    }

    /// Method for checking if a SafeQueue is empty.
    bool empty()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        return _rawQueue.empty();
    }

    /// Method for return size.
    size_t size()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        return _rawQueue.size();
    }

    /// Method for clear SafeQueue.
    void clear()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        _rawQueue.clear();
    }

    /// Method to wait which uses condition variable.
    void wait()
    {
        while (empty())
        {
            std::unique_lock<std::mutex> uniqueLock(_uniqueMutex);
            _block.wait(uniqueLock);
        }
    }
};
}  /// namespace Network
