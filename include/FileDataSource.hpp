#pragma once

#include <fstream>
#include <vector>
#include <string>

#include "DataSource.hpp"
#include "RingBufferQueue.hpp"


class FileDataSource : public DataSource {
private:
    std::string filename_;
public:
    explicit FileDataSource(const std::string &filename);
    void fetch_data(RingBufferQueue<std::string> &lines, std::atomic<bool>& stop_flag) override;
};