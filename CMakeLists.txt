cmake_minimum_required(VERSION 3.10)
project(RealTimeDataPipeline)

set(CMAKE_CXX_STANDARD 17)

add_executable(RealTimeDataPipeline
    src/main.cpp
    src/FileDataSource.cpp
    src/DataProcessor.cpp
    src/Logger.cpp
    src/OutputWriter.cpp
    src/ConfigManager.cpp
    src/DataSourceFactory.cpp
    src/APIDataSource.cpp
    src/TrimAndTagStrategy.cpp
    src/UpperCaseStrategy.cpp
    src/CommandLineParser.cpp
    src/SignalHandler.cpp
    src/ThreadPool.cpp
)

target_include_directories(RealTimeDataPipeline PRIVATE include)

include(FetchContent)

# ---- libcurl ----
find_package(CURL REQUIRED)
target_include_directories(RealTimeDataPipeline PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(RealTimeDataPipeline PRIVATE ${CURL_LIBRARIES})

enable_testing()
add_executable(DataProcessorTests 
    tests/test_DataProcessor.cpp
    src/DataProcessor.cpp
    src/TrimAndTagStrategy.cpp
    src/UpperCaseStrategy.cpp
    tests/test_FileDataSource.cpp
    src/FileDataSource.cpp
    src/APIDataSource.cpp
    src/DataSourceFactory.cpp
    src/ConfigManager.cpp
    tests/test_APIDataSource.cpp
    tests/test_FullPipeline_File.cpp
    src/OutputWriter.cpp
    tests/test_Logger.cpp
    src/Logger.cpp
)

target_link_libraries(DataProcessorTests gtest gtest_main pthread ${CURL_LIBRARIES})
target_include_directories(DataProcessorTests PRIVATE include)
add_test(NAME DataProcessorTests COMMAND DataProcessorTests)
# set(CMAKE_BUILD_TYPE Debug)

