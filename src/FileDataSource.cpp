#include "FileDataSource.hpp"
#include <fstream>
#include <iostream>

FileDataSource::FileDataSource(const std::string &filename) : filename_(filename) {}

void FileDataSource::fetch_data(ThreadSafeQueue<std::string> &lines)
{
    std::ifstream file(filename_);
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename_ << std::endl;
    }

    while (std::getline(file, line))
    {
        lines.push(line);
    }

}