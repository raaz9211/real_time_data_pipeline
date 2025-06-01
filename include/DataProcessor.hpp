#pragma once

#include <string>
#include <vector>
#include <atomic>

#include "ThreadSafeQueue.hpp"



class DataProcessor {
public:
    std::string process_line(const std::string &line);
    void consume(ThreadSafeQueue<std::string>& input_queue, std::atomic<bool>& done_flag, ThreadSafeQueue<std::string>& output_queue);
};