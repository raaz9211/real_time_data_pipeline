#pragma once
#include <string>
#include <fstream>
#include <mutex>

class OutputWriter {
public:
    OutputWriter(const std::string& filename);
    ~OutputWriter();
    void write(const std::string &data);
private:
    std::ofstream output_file;
    std::mutex write_mutex;  // Mutex to protect output_file writes

};