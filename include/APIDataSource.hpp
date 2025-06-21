#pragma once

#include <string>

#include "DataSource.hpp"
#include "RingBufferQueue.hpp"

class APIDataSource : public DataSource
{
private:
    std::string url_;

public:
    explicit APIDataSource(const std::string &url);
    void fetch_data(RingBufferQueue<std::string> &queue, std::atomic<bool>& stop_flag) override;
};