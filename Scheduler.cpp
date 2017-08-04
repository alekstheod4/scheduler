#include "Scheduler.h"
#include <chrono>
#include <iostream>
#include <future>

namespace
{
    using TaskId = Scheduler::TaskId;
}

void Scheduler::run()
{
    while(m_keepRunning)
    {
        std::lock_guard<std::mutex> lock(m_lock);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for(auto& task : m_tasks)
        {
            auto now = std::time(nullptr);
            if((now - task.m_lastRun)>task.m_interval.count())
            {
                std::thread(task).detach();
                task.m_lastRun = now;
            }
        }
    }
}

Scheduler::Scheduler() : m_keepRunning(true), m_thread([this](){this->run();})
{
    m_thread.detach();
}

Scheduler::~Scheduler()
{
    std::lock_guard<std::mutex> lock(m_lock);
    m_keepRunning = false;
    m_thread.join();
}

TaskId Scheduler::add(const Task& newTask)
{
    std::lock_guard<std::mutex> lock(m_lock);
    TaskId result;
    ///TODO linear search might be optimized if required
    for(int i = 0; i < m_tasks.size(); i++)
    {
        if(!m_tasks[i].m_run)
        {
            result = TaskId{i};
            m_tasks[i] = newTask;
        }
    }
    
    if(result.value < 0)
    {
        m_tasks.push_back(newTask);
        result.value = m_tasks.size() - 1;
    }
    
    return result;
}

void Scheduler::remove(const TaskId& id)
{
    std::lock_guard<std::mutex> lock(m_lock);
    if(id.value < m_tasks.size())
    {
        m_tasks[id.value] = Task{};
    }
}
