#pragma once
#include <mutex>

#include "SafePriorityQueue.hpp"
#include "Task.hpp"

class ThreadPool
{
public:
    explicit ThreadPool(const size_t threadCount)
    {
        _workers.reserve(threadCount);

        for (size_t i = 0; i < threadCount; ++i)
        {
            _workers.emplace_back(&ThreadPool::processTasks, this);
        }
    }

    ~ThreadPool()
    {
        {
            std::lock_guard<std::mutex> guard(_mutex);

            _stopRequested = true;
        }

        _cv.notify_all();

        for (auto& worker : _workers)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }

    void execute(Task&& task)
    {
        {
            std::lock_guard<std::mutex> guard(_mutex);
            _tasks.push(std::move(task));
        }

        _cv.notify_one();
    }

private:
    void processTasks()
    {
        if (_tasks.empty())
        {
            return;
        }

        std::unique_lock<std::mutex> lock(_mutex);

        auto& task = _tasks.top();

        lock.unlock();

        task();
    }

private:
    SafePriorityQueue<Task, Task::Comporator> _tasks;
    std::vector<std::thread> _workers;
    std::condition_variable _cv;
    std::mutex _mutex;
};
