#include <gtest/gtest.h>
#include <memory>
#include <fstream>
#include <filesystem>

#include "DataSourceFactory.hpp"
#include "ThreadSafeQueue.hpp"
#include "ConfigManager.hpp"



namespace fs = std::filesystem;

TEST(FileDataSourceTest, FetchesLinesFromFile) {
    // Arrange
    std::string testFile = "../tests/test_file.txt";
    std::ofstream out(testFile);
    out << "line1\nline2\nline3";
    out.close();

    ConfigManager config("../tests/test_config.json");
    // We override config filePath by mocking the method or modifying config directly (depending on your implementation)
    // Here, we simulate that config.getFilePath() will return our test file path.

    // You can refactor ConfigManager or inject test config directly.

    // Act
    auto dataSource = DataSourceFactory::create("file", config);
    ThreadSafeQueue<std::string> queue;
    std::atomic<bool> stop_flag = false;
    dataSource->fetch_data(queue, stop_flag);

    // Assert
    ASSERT_FALSE(queue.empty());
    std::string value;
    queue.try_pop(value);
    EXPECT_EQ(value, "line1");
    queue.try_pop(value);
    EXPECT_EQ(value, "line2");
    queue.try_pop(value);
    EXPECT_EQ(value, "line3");

    fs::remove(testFile);
}

TEST(DataSourceFactoryTest, ThrowsOnInvalidType) {
    ConfigManager config("../tests/test_config.json");
    EXPECT_THROW({
        auto dataSource = DataSourceFactory::create("invalid_type", config);
},std::invalid_argument);
}