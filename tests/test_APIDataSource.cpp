#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdlib>

#include "APIDataSource.hpp"
#include "ThreadSafeQueue.hpp"

TEST(APIDataSourceTest, FetchesJsonDataSuccessfully) {
    // Start a local HTTP server (Python) that returns a JSON array
    std::string cmd = "python3 -m http.server 8000 --directory ../tests/mock_api_data &";
    std::system(cmd.c_str());

    // Give server time to start
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Point APIDataSources to localhost
    std::string url = "http://localhost:8000/mock_data.json";
    APIDataSource apiDataSource(url);

    ThreadSafeQueue<std::string> queue;
    std::atomic<bool> stop_flag = false;
    apiDataSource.fetch_data(queue, stop_flag);

    ASSERT_FALSE(queue.empty());
    std::string item;
    queue.try_pop(item);
    ASSERT_TRUE(item.find("\"name\":") != std::string::npos);

    // Kill the server (optional, depends on OS)
    std::system("fuser -k 8000/tcp > /dev/null 2>&1");

} 