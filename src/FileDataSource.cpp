#include "FileDataSource.hpp"
#include <fstream>
#include <iostream>

FileDataSource::FileDataSource(const std::string &filename) : filename_(filename) {}

void FileDataSource::fetch_data(RingBufferQueue<std::string> &lines, std::atomic<bool>& stop_flag)
{
    std::ifstream file(filename_);
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename_ << std::endl;
    }

    while (!stop_flag && std::getline(file, line))
    {
        lines.push(line);
    }

}