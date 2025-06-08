#include "ThreadPool.hpp"


ThreadPool::ThreadPool(size_t threadCount) : stop(false) {
    for(size_t i = 0; i < threadCount; i++) {
        workers.emplace_back([this]() { worker_loop(); });
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::shutdown() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;
    }
    cv.notify_all();
    for(auto &thread : workers) {
        if(thread.joinable()){
            thread.join();
        }
    }
}

void ThreadPool::worker_loop() {
    while(true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this]() {return stop || !tasks.empty(); });

            if (stop and tasks.empty()){
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}

