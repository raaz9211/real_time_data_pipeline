#pragma once

#include <vector>
#include <mutex>
#include <string>
#include <condition_variable>

template<typename T>
class RingBufferQueue {
public:
    explicit RingBufferQueue(size_t capacity);

    void push(const T &item);

    void pop(T &item);

    bool try_push(const T &item);

    bool try_pop(T &inem);

    size_t size() const;

    bool empty() const;

    bool full() const;

private:
    std::vector<T> buffer;
    size_t capacity;
    size_t head = 0;
    size_t tail = 0;
    size_t count = 0;

    mutable std::mutex mtx;
    std::condition_variable not_full;
    std::condition_variable not_empty;
};
