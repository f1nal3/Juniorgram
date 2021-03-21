#pragma once
#include <queue>
#include <vector>

#include "Message.hpp"
#include "PriorityQueue.hpp"

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

    PriorityQueue<network::Message, TasksComporator> tasks;

public:
    TasksManager()
    {
        if (!tasks.empty())
        {
            network::Message message = tasks.top();
            // process task ... (send to server/client)

            tasks.pop();
        }
    }

    void addTask(network::Message& message) { tasks.push(message); }
};
