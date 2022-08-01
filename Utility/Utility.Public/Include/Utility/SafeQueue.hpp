#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>


namespace Utility
{
template <typename T>
/**
 * @brief The template SafeQueue class
 */
class SafeQueue
{
private:
    std::mutex              _scopedMutex, _uniqueMutex;
    std::condition_variable _block;

    std::deque<T> _rawQueue;

public:
    /**
     * @Default SafeQueue constructor
     */
    SafeQueue() = default;

    /** 
     * @SafeQueue copy constructor delete
     */ 
    SafeQueue(const SafeQueue<T>& other) = delete;

    /**
     * @SafeQueue destructor for clear method
     */
    ~SafeQueue() { clear(); }

    /**
     * @Method for access to front value
     */
    const T& front()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        return _rawQueue.front();
    }

    /**
     * @Method for access to back value
     */
    const T& back()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        return _rawQueue.back();
    }

    /**
     * @Method to remove front value
     */
    T pop_front()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        T                result = std::move(_rawQueue.front());
        _rawQueue.pop_front();
        return result;
    }

    /** 
     * @Method to remove back value
     */
    T pop_back()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        T                result = std::move(_rawQueue.back());
        _rawQueue.pop_back();
        return result;
    }

    /** 
     * @Method for adding a value to the end
     */
    void push_back(const T& item)
    {
        std::scoped_lock scopedLock(_scopedMutex);
        _rawQueue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> uniqueLock(_uniqueMutex);
        _block.notify_one();
    }

    /** 
     * @Method to add front value
     */
    void push_front(const T& item)
    {
        std::scoped_lock scopedLock(_scopedMutex);
        _rawQueue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> uniqueLock(_uniqueMutex);
        _block.notify_one();
    }

    /**
     * @Method for checking if the SafeQueue is empty
     */
    bool empty()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        return _rawQueue.empty();
    }

    /**
     * @Method for return size
     */
    size_t size()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        return _rawQueue.size();
    }

    /**
     * @Method for clear SafeQueue
     */
    void clear()
    {
        std::scoped_lock scopedLock(_scopedMutex);
        _rawQueue.clear();
    }

    /**
     * @Method to wait which uses condition variable
     */
    void wait()
    {
        while (empty())
        {
            std::unique_lock<std::mutex> uniqueLock(_uniqueMutex);
            _block.wait(uniqueLock);
        }
    }
};
}  /// namespace Utility
