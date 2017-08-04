#pragma once

#include <functional>
#include <vector>
#include <thread>
#include <mutex>

struct Task{
    Task(): m_lastRun(0), m_interval(0){
    }
    
    Task(std::chrono::seconds interval, const std::function<void()>& run):
                                            m_lastRun(0), 
                                            m_interval(interval),
                                            m_run(run)
    {
    }
    
    unsigned int m_lastRun;
    std::chrono::seconds m_interval;
    std::function<void()> m_run;
    void operator()(){
        m_run();
    }
};

class Scheduler
{
public:
    struct TaskId
    {
        int value = -1;
    };    
    
    TaskId add(const Task& task);
    void remove(const TaskId& id);
    
    Scheduler();
    ~Scheduler();
    
private:
    Scheduler(const Scheduler&);
    void run();
    
    std::vector<Task> m_tasks;
    std::thread m_thread;
    std::mutex m_lock;
    bool m_keepRunning;
};
