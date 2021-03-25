#pragma once
#include <functional>
#include <queue>
#include <mutex>
#include <vector>

#include "SafePriorityQueue.hpp"
#include "Task.h"

class TaskManager
{
public:
    explicit TaskManager(const size_t threadCount)
    {
        mWorkers.reserve(threadCount);

        for (size_t i = 0; i < threadCount; ++i)
        {
            mWorkers.emplace_back(&TaskManager::processTasks, this);
        }
    }

    void run()
    {
        mStopped = false;
        mBlock.notify_all();
    }

    void stop() { mStopped = true; }

    void addTask(Task&& task)
    {
        std::lock_guard<std::mutex> guard(mMutex);
        mTasks.push(std::move(task));
    }

    ~TaskManager()
    {
        mBlock.notify_all();

        for (auto& worker : mWorkers)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }

private:
    void processTasks()
    {
        std::unique_lock<std::mutex> lk(mMutex);

        mBlock.wait(lk, [this] { return !mStopped && !mTasks.empty(); });
        // std::cout << std::this_thread::get_id() << std::endl;
        Task task = mTasks.pop();
        task();
        // std::cout << std::endl;

        lk.unlock();
        processTasks();
    }

private:
    bool mStopped = true;
    std::condition_variable mBlock;
    std::mutex mMutex;
    std::vector<std::thread> mWorkers;
    SafePriorityQueue<Task, Task::Comporator> mTasks;
};
