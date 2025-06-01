#pragma once

#include <string>
#include <vector>

#include "ThreadSafeQueue.hpp"

class DataSource {
public:
    virtual void fetch_data(ThreadSafeQueue<std::string> &lines)  = 0;
    virtual ~DataSource() = default;
};