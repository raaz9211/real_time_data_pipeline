#include "Logger.hpp"

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
    std::string full_mgs = "[" + current_timestamp() + "] [" + level_to_string(level) + "] " + message;

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

std::string Logger::current_timestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t now_time = system_clock::to_time_t(now);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now_time));
    return std::string(buffer);

}