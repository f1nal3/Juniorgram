#include "SafeQueue.hpp"    

//namespace Network
//{
//    template<class T>
//    const T& SafeQueue<T>::front()
//    {
//        std::scoped_lock scopedLock(mScopedMutex);
//        return mRawQueue.front();
//    }
//
//    template <class T>
//    const T& SafeQueue<T>::back()
//    {
//        std::scoped_lock scopedLock(mScopedMutex);
//        return mRawQueue.back();
//    }
//
//    template <class T>
//    T SafeQueue<T>::pop_front()
//    {
//        std::scoped_lock scopedLock(mScopedMutex);
//        T result = std::move(mRawQueue.front());
//        mRawQueue.pop_front();
//        return result;
//    }
//
//    template <class T>
//    T SafeQueue<T>::pop_back()
//    {
//        std::scoped_lock scopedLock(mScopedMutex);
//        T result = std::move(mRawQueue.back());
//        mRawQueue.pop_back();
//        return result;
//    }
//
//    template <class T>
//    void SafeQueue<T>::push_back(const T& item)
//    {
//        std::scoped_lock scopedLock(mScopedMutex);
//        mRawQueue.emplace_back(std::move(item));
//
//        std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
//        mBlock.notify_one();
//    }
//
//    template <class T>
//    void SafeQueue<T>::push_front(const T& item)
//    {
//        std::scoped_lock scopedLock(mScopedMutex);
//        mRawQueue.emplace_front(std::move(item));
//
//        std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
//        mBlock.notify_one();
//    }
//
//    template <class T>
//    bool SafeQueue<T>::empty()
//    {
//        std::scoped_lock scopedLock(mScopedMutex);
//        return mRawQueue.empty();
//    }
//
//    template <class T>
//    size_t SafeQueue<T>::size()
//    {
//        std::scoped_lock scopedLock(mScopedMutex);
//        return mRawQueue.size();
//    }
//
//    template <class T>
//    void SafeQueue<T>::clear()
//    {
//        std::scoped_lock scopedLock(mScopedMutex);
//        mRawQueue.clear();
//    }
//
//    template <class T>
//    void SafeQueue<T>::wait()
//    {
//        while (empty())
//        {
//            std::unique_lock<std::mutex> uniqueLock(mUniqueMutex);
//            mBlock.wait(uniqueLock);
//        }
//    }
//
//    template <class T>
//    SafeQueue<T>::~SafeQueue() { clear(); }
//    
//}
