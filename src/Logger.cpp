#include "Logger.hpp"
#include "Utils.hpp"


Logger &Logger::instance()
{
    static Logger logger;
    return logger;
}


void Logger::set_log_file(const std::string &filename)
{
    std::lock_guard<std::mutex> lock(log_mutex);
    if (log_file.is_open())
    {
        log_file.close();
    }
    log_file.open(filename, std::ios::out | std::ios::app);
}



void Logger::log(LogLevel level, const std::string &message) {
    std::lock_guard<std::mutex> lock(log_mutex);
    std::string full_mgs = "[" + Utils::current_timestamp() + "] [" + level_to_string(level) + "] " + message;

    if (log_file.is_open()) {
        log_file << full_mgs << std::endl;
    } else {
        std::cout << full_mgs << std::endl;

    }
}

std::string Logger::level_to_string(LogLevel level) {
    switch (level)
    {
        case LogLevel::INFO: return "INFO";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

