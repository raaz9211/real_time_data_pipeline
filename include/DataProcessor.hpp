#pragma once

#include <string>
#include <vector>
#include <atomic>

#include "ThreadSafeQueue.hpp"
#include "ProcessingStrategy.hpp"



class DataProcessor {
private:
     std::unique_ptr<ProcessingStrategy> strategy_;
public:
    explicit DataProcessor(std::unique_ptr<ProcessingStrategy> strategy);
    void consume(ThreadSafeQueue<std::string>& input_queue, std::atomic<bool>& done_flag, ThreadSafeQueue<std::string>& output_queue);
};