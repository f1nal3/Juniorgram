#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

namespace Utility
{
template <typename T>
/**
 * @brief The tamplate SafeQueue class
 */
class SafeQueue
{
private:
    std::mutex              _ScopedMutex, _UniqueMutex;
    std::condition_variable _Block;

    std::deque<T> _RawQueue;

public:
    /// Default SafeQueue constructor
    SafeQueue() = default;

    /// SafeQueue copy constructor delete
    SafeQueue(const SafeQueue<T>& other) = delete;

    /// SafeQueue destructor for clear method
    ~SafeQueue() { clear(); }

    /// Method for access to front value
    const T& front()
    {
        std::scoped_lock scopedLock(_ScopedMutex);
        return _RawQueue.front();
    }

    /// Method for access to back value
    const T& back()
    {
        std::scoped_lock scopedLock(_ScopedMutex);
        return _RawQueue.back();
    }

    /// Method to remove front value
    T pop_front()
    {
        std::scoped_lock scopedLock(_ScopedMutex);
        T                result = std::move(_RawQueue.front());
        _RawQueue.pop_front();
        return result;
    }

    /// Method to remove back value
    T pop_back()
    {
        std::scoped_lock scopedLock(_ScopedMutex);
        T                result = std::move(_RawQueue.back());
        _RawQueue.pop_back();
        return result;
    }

    /// Method to add back value
    void push_back(const T& item)
    {
        std::scoped_lock scopedLock(_ScopedMutex);
        _RawQueue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> uniqueLock(_UniqueMutex);
        _Block.notify_one();
    }

    /// Method to add front value
    void push_front(const T& item)
    {
        std::scoped_lock scopedLock(_ScopedMutex);
        _RawQueue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> uniqueLock(_UniqueMutex);
        _Block.notify_one();
    }

    /// Method for checking if a SafeQueue is empty
    bool empty()
    {
        std::scoped_lock scopedLock(_ScopedMutex);
        return _RawQueue.empty();
    }

    /// Method for return size
    size_t size()
    {
        std::scoped_lock scopedLock(_ScopedMutex);
        return _RawQueue.size();
    }

    /// Method for clear SafeQueue
    void clear()
    {
        std::scoped_lock scopedLock(_ScopedMutex);
        _RawQueue.clear();
    }

    /// Method to wait which uses condition variable
    void wait()
    {
        while (empty())
        {
            std::unique_lock<std::mutex> uniqueLock(_UniqueMutex);
            _Block.wait(uniqueLock);
        }
    }
};
}  // namespace Utility
