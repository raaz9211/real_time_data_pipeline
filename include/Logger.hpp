#pragma once
#include <string>
#include <mutex>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>

enum class LogLevel
{
    INFO,
    DEBUG,
    WARNING,
    ERROR
};

class Logger
{

private:
    Logger() = default;
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    std::mutex log_mutex;
    std::ofstream log_file;
    std::string level_to_string(LogLevel level);

public:
    static Logger &instance();
    void log(LogLevel level, const std::string &filename);
    void set_log_file(const std::string &filename);
};
