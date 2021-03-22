#pragma once
#include <functional>

class Task
{
public:
    enum class Priority
    {
        HIGH   = 0,
        MEDIUM = 1,
        LOW    = 2,
    };

    Task(std::function<void()> task, const Priority importance) : task(task), importance(importance)
    {

    }

    Priority getImportance() const { return importance; }

    void operator()() { task(); }

public:
    struct Comporator
    {
        bool operator()(const Task& first, const Task& second) const
        {
            return first.importance() > second.importance();
        }
    };

private:
    std::function<void()> task;
    Priority importance;
};
