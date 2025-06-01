#pragma once

#include <fstream>
#include <vector>
#include <string>

#include "DataSource.hpp"
#include "ThreadSafeQueue.hpp"


class FileDataSource : public DataSource {
private:
    std::string filename_;
public:
    explicit FileDataSource(const std::string &filename);
    void fetch_data(ThreadSafeQueue<std::string> &lines) override;    
};