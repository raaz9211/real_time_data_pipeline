#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>

class ThreadPool
{
public:
    explicit ThreadPool(size_t threadCount);
    ~ThreadPool();

    // Submit a task and get a future result
    template <typename Func, typename... Args>
    auto submit(Func &&func, Args &&...args) -> std::future<decltype(func(args...))>;
    void shutdown();

private:
    void worker_loop();
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<bool> stop;
};

template <typename Func, typename... Args>
auto ThreadPool::submit(Func &&func, Args &&...args) -> std::future<decltype(func(args...))>
{
    using ReturnType = decltype(func(args...));

    auto task_ptr = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

    {
        std::lock_guard<std::mutex> lock(mutex);
        if (stop)
        {
            throw std::runtime_error("ThreadPool is shutting down");
        }
        tasks.emplace([task_ptr]()
                      { (*task_ptr)(); });
    }

    cv.notify_one();
    return task_ptr->get_future();
}
