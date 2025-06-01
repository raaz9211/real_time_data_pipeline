#include "OutputWriter.hpp"
#include "Utils.hpp"

OutputWriter::OutputWriter(const std::string &filename, OutputMode mode, bool useJson, bool addTimestamps) : mode(mode), useJsonFormate(useJson), addTimestamps(addTimestamps)
{
    if (mode == OutputMode::File)
    {
        output_file.open(filename, std::ios::out | std::ios::app);
    }
}

OutputWriter::~OutputWriter()
{
    if (output_file.is_open())
    {
        output_file.close();
    }
}

void OutputWriter::write(const std::string &data)
{
    std::lock_guard<std::mutex> lock(write_mutex);
    std::string output = formatOutput(data);

    if (mode == OutputMode::File and output_file.is_open())
    {
        output_file << output << std::endl;
    } else if (mode == OutputMode::StdOut) {
        std::cout << output << std::endl;
    }
}

std::string OutputWriter::formatOutput(const std::string& data) const {
    std::ostringstream oss;

    if(addTimestamps) {
        oss << "[ " << Utils::current_timestamp() << "] ";
    }

    if(useJsonFormate) {
        oss << "{ \"data\": \"" << data << "\" }";
    } else {
        oss << data;
    }

    return oss.str();
}