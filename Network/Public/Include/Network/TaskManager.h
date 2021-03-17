#pragma once
#include <queue>
#include <vector>

#include "Message.hpp"

class TasksManager
{
private:
    struct TasksComporator
    {
        bool operator()(const network::Message& first, const network::Message& second) const
        {
            return first.mHeader.mID < second.mHeader.mID;
        }
    };

    std::priority_queue<network::Message, std::vector<network::Message>, TasksComporator> tasks;

public:
    TasksManager()
    {
        if (!tasks.empty())
        {
            network::Message message = tasks.top();
        }
    }

    void addTask(network::Message& message) { tasks.push(message); }
};
