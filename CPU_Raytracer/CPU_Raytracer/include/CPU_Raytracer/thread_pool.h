#pragma once
#include <iostream>
#include <thread>
#include <functional>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    void enqueue(std::function<void()> func);
    void waitUntilDone();

private:
    void workerThread();
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::condition_variable doneCondition;
    bool stop;
    size_t unfinishedTasks;
    std::mutex outputMutex; // Mutex for synchronizing output
};