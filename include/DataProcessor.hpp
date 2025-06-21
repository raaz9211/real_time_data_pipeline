#pragma once

#include <string>
#include <vector>
#include <atomic>

#include "RingBufferQueue.hpp"
#include "ProcessingStrategy.hpp"



class DataProcessor {
private:
     std::unique_ptr<ProcessingStrategy> strategy_;
public:
    explicit DataProcessor(std::unique_ptr<ProcessingStrategy> strategy);
    void consume(RingBufferQueue<std::string>& input_queue, std::atomic<bool>& done_flag, RingBufferQueue<std::string>& output_queue);
};