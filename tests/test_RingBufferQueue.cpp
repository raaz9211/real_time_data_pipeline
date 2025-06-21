// tests/test_RingBufferQueue.cpp
#include "RingBufferQueue.hpp"
#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <vector>


TEST(RingBufferQueueTest, PushAndPopSingleItem) {
    RingBufferQueue<std::string> queue(4);
    queue.push("hello");

    std::string out;
    queue.pop(out);
    EXPECT_EQ(out, "hello");

}

TEST(RingBufferQueueTest, FIFOOrder) {
    RingBufferQueue<int> queue(5);
    queue.push(1);
    queue.push(2);
    queue.push(3);

    int x;
    queue.pop(x);
    EXPECT_EQ(x, 1);
    queue.pop(x);
    EXPECT_EQ(x, 2);
    queue.pop(x);
    EXPECT_EQ(x, 3);
}

TEST(RingBufferQueueTest, WrapAroundBehavior) {
    RingBufferQueue<int> queue(3);
    queue.push(1);
    queue.push(2);
    int x;
    queue.pop(x); // now one free spot
    queue.push(3); // should go to beginning if implemented correctly

    queue.pop(x);
    EXPECT_EQ(x, 2);
    queue.pop(x);
    EXPECT_EQ(x, 3);
}

TEST(RingBufferQueueTest, QueueSizeCheck) {
    RingBufferQueue<int> queue(4);
    EXPECT_EQ(queue.size(), 0);
    queue.push(5);
    queue.push(6);
    EXPECT_EQ(queue.size(), 2);
    int x;
    queue.pop(x);
    EXPECT_EQ(queue.size(), 1);
}

// try function
TEST(RingBufferQueueTest, TryPushSucceedsWhenNotFull) {
    RingBufferQueue<int> queue(2);
    EXPECT_TRUE(queue.try_push(100));
    EXPECT_TRUE(queue.try_push(200));
}

TEST(RingBufferQueueTest, TryPushFailsWhenFull) {
    RingBufferQueue<int> queue(2);
    EXPECT_TRUE(queue.try_push(1));
    EXPECT_TRUE(queue.try_push(2));
    EXPECT_FALSE(queue.try_push(3)); // Should fail, queue is full
}

TEST(RingBufferQueueTest, TryPopSucceedsWhenNotEmpty) {
    RingBufferQueue<std::string> queue(2);
    queue.try_push("hello");
    std::string result;
    EXPECT_TRUE(queue.try_pop(result));
    EXPECT_EQ(result, "hello");
}

TEST(RingBufferQueueTest, TryPopFailsWhenEmpty) {
    RingBufferQueue<int> queue(2);
    int out;
    EXPECT_FALSE(queue.try_pop(out)); // Nothing to pop
}

TEST(RingBufferQueueTest, TryPushPopAlternating) {
    RingBufferQueue<int> queue(3);
    EXPECT_TRUE(queue.try_push(1));
    int out;
    EXPECT_TRUE(queue.try_pop(out));
    EXPECT_EQ(out, 1);
    EXPECT_TRUE(queue.try_push(2));
    EXPECT_TRUE(queue.try_push(3));
    EXPECT_TRUE(queue.try_pop(out));
    EXPECT_EQ(out, 2);
    EXPECT_TRUE(queue.try_pop(out));
    EXPECT_EQ(out, 3);
    EXPECT_FALSE(queue.try_pop(out)); // Empty now
    EXPECT_EQ(queue.size(), 0);

}

TEST(RingBufferQueueTest, TryPushPopMultiThreaded) {
    RingBufferQueue<int> queue(100);
    std::atomic<int> produced = 0;
    std::atomic<int> consumed = 0;

    // Producer thread
    std::thread producer([&]() {
        for (int i = 0; i < 1000; ++i) {
            if (queue.try_push(i)) {
                produced++;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    });

    // Consumer thread
    std::thread consumer([&]() {
        int val;
        for (int i = 0; i < 1000; ++i) {
            if (queue.try_pop(val)) {
                consumed++;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    });

    producer.join();
    consumer.join();

    EXPECT_GE(produced.load(), consumed.load()); // Can't consume more than produced
}
