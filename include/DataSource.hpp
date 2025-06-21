#pragma once

#include <string>
#include <vector>
#include <atomic>

#include "RingBufferQueue.hpp"

class DataSource {
public:
    virtual void fetch_data(RingBufferQueue<std::string> &lines, std::atomic<bool>& stop_flag)  = 0;
    virtual ~DataSource() = default;
};