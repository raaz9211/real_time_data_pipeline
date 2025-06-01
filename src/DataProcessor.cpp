#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

#include "DataProcessor.hpp"

void DataProcessor::consume(ThreadSafeQueue<std::string> &input_queue, std::atomic<bool> &done_flag, ThreadSafeQueue<std::string> &output_queue)
{
    while (!done_flag || !input_queue.empty())
    {
        std::string line;
        if (input_queue.try_pop(line))
        {
            std::string result = process_line(line);
            // std::cout << result << std::endl;
            output_queue.push(result);
        }
    }
}

std::string DataProcessor::process_line(const std::string &line)
{
    std::string trimmed = line;
    trimmed.erase(trimmed.find_last_not_of(" \n\r\t") + 1);
    trimmed.erase(0, trimmed.find_first_not_of(" \n\r\t"));
    return "[Processed] " + trimmed;
}