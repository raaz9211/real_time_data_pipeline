#pragma once

#include <string>

#include "DataSource.hpp"
#include "ThreadSafeQueue.hpp"

class APIDataSource : public DataSource
{
private:
    std::string url_;

public:
    explicit APIDataSource(const std::string &url);
    void fetch_data(ThreadSafeQueue<std::string> &queue) override;
};