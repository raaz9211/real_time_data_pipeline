#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

class OutputWriter
{
public:
    enum class OutputMode
    {
        File,
        StdOut
    };
    OutputWriter(const std::string &filePath, OutputMode outputMode = OutputMode::File, bool userJson = false, bool addTimestamps = false);
    ~OutputWriter();
    void write(const std::string &data);

private:
    std::ofstream output_file;
    std::mutex write_mutex; // Mutex to protect output_file writes
    OutputMode mode;
    bool useJsonFormate;
    bool addTimestamps;

    std::string formatOutput(const std::string &data) const;
};