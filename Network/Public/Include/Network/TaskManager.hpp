#pragma once
#include <future>
#include <queue>
#include <vector>

#include "Message.hpp"
#include "PriorityQueue.hpp"
#include "Task.hpp"
#include "ThreadPool.hpp"
#include "SafeQueue.hpp"

class TasksManager
{
public:
    TasksManager(const std::shared_ptr<ThreadPool>& threadpool, const size_t maxWorkers)
        : _threadpool(std::move(threadpool)), _maxWorkers(maxWorkers){}
    {
    }

    void addTask(std::future<void()>&& task)
    {
        std::lock_guard<std::mutex> guard(_mutex);

        if (_stopped)
        {
            return;
        }
        _tasks.push(std::move(task));

        this->processTasks();
    }

private:
    void processTasks()
    {
        if (_tasks.empty() || _workerCount == _maxWorkers)
        {
            return;
        }

        auto task = std::move(_tasks.front());
        _tasks.pop();

        ++_workerCount;
        _threadpool->execute(std::move(task));
    }

private:
    size_t _maxWorkers;
    size_t _workerCount = 0;
    std::mutex _mutex;
    std::shared_ptr<ThreadPool> _threadpool;
    std::queue<Task> _tasks;
};
