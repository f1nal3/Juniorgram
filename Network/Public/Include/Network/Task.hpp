#pragma once
#include <functional>

class Task
{
public:
    enum class Priority : std::uint8_t
    {
        HIGH   = 0,
        MEDIUM = 1,
        LOW    = 2,
    };

    Task(std::function<void()> task, const Priority importance)
        : mTask(task), mImportance(importance)
    {
    }

    Priority getImportance() const { return mImportance; }

    void operator()() { mTask(); }

public:
    struct Comporator
    {
        bool operator()(const Task& first, const Task& second) const
        {
            return first.getImportance() > second.getImportance();
        }
    };

private:
    std::function<void()> mTask;
    Priority mImportance;
};
