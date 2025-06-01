#include "OutputWriter.hpp"

OutputWriter::OutputWriter(const std::string &filename) {
    output_file.open(filename, std::ios::out | std::ios::app);
}

OutputWriter::~OutputWriter() {
    if (output_file.is_open()) {
        output_file.close();
    }
}

void OutputWriter::write(const std::string &data) {
    std::lock_guard<std::mutex> lock(write_mutex);
    if (output_file.is_open()) {
        output_file << data << std::endl;
    }
}