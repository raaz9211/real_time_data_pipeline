#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

#include "Logger.hpp"
#include "Utils.hpp"

namespace fs = std::filesystem;

TEST(LoggerTest, LogToFile) {
    std::string logPath = "../tests/test_logger_output.log";
    Logger::instance().set_log_file(logPath);
    Logger::instance().log(LogLevel::INFO, "Test logging line");

    std::ifstream in(logPath);
    ASSERT_TRUE(in.is_open());

    std::string lastLine;
    while (in) {
        std::getline(in, lastLine);
        break;
    }
    EXPECT_NE(lastLine.find("Test logging line"), std::string::npos);
    // EXPECT_NE(lastLine.find("INFO"), std::string::npos);
    fs::remove(logPath);

}