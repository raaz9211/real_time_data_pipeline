#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue
{
private:
    mutable std::mutex mtx_;
    std::queue<T> queue_;
    std::condition_variable cond_var_;

public:
    void push(const T &value)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push(value);
        cond_var_.notify_one();
    }

    bool try_pop(T &result)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.empty())
        {
            return false;
        }
        result = queue_.front();
        queue_.pop();
        return true;
    }

    void wait_and_pop(T &result)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cond_var_.wait(lock, [this]
                       { return !queue_.empty(); });
        result = queue_.front();
        queue_.pop();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.empty();
    }

    std::size_t size() const {
        return queue_.size();
    }
};