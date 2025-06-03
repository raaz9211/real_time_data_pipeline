#include "DataProcessor.hpp"

DataProcessor::DataProcessor(std::unique_ptr<ProcessingStrategy> strategy) : strategy_(std::move(strategy)) {}

void DataProcessor::consume(ThreadSafeQueue<std::string> &input_queue, std::atomic<bool> &done_flag, ThreadSafeQueue<std::string> &output_queue)
{
    while (!done_flag || !input_queue.empty())
    {
        std::string line;
        if (input_queue.try_pop(line))
        {
            std::string result = strategy_->process(line);
            output_queue.push(result);
        }
    }
}

