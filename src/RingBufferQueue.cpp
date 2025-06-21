// src/RingBufferQueue.cpp
#include "RingBufferQueue.hpp"

template<typename T>
RingBufferQueue<T>::RingBufferQueue(size_t capacity) : buffer(capacity), capacity((capacity)) {}

template<typename T>
void RingBufferQueue<T>::push(const T &item) {
    std::unique_lock<std::mutex> lock(mtx);
    not_full.wait(lock, [this]() {return count < capacity;});

    buffer[tail] = item;
    tail = (tail + 1) % capacity;
    count += 1;

    not_empty.notify_one();
}

template<typename T>
void RingBufferQueue<T>::pop(T &item) {
    std::unique_lock<std::mutex> lock(mtx);
    not_empty.wait(lock, [this]() {return count > 0;});

    item = buffer[head];
    head = (head + 1) % capacity;
    count -= 1;

    not_full.notify_one();
}

template<typename T>
bool RingBufferQueue<T>::try_push(const T &item) {
    std::unique_lock<std::mutex> lock(mtx);

    if (count == capacity) {
        return false;
    }

    buffer[tail] = item;
    tail = (tail + 1) % capacity;
    count += 1;

    not_empty.notify_one();
    return true;
}

template<typename T>
bool RingBufferQueue<T>::try_pop(T &item) {
    std::unique_lock<std::mutex> lock(mtx);

    if (count == 0) {
        return false;
    }

    item = buffer[head];
    head = (head + 1) % capacity;
    count -= 1;

    not_full.notify_one();
    return true;
}

template<typename T>
size_t RingBufferQueue<T>::size() const {
    std::lock_guard<std::mutex> lock(mtx);
    return count;
}

template<typename T>
bool RingBufferQueue<T>::empty() const {
    return size() == 0;
}

template<typename T>
bool RingBufferQueue<T>::full() const {
    return size() == capacity;
}

// Required: Explicit instantiation for types used
template class RingBufferQueue<std::string>;
template class RingBufferQueue<int>;