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
)

target_include_directories(RealTimeDataPipeline PRIVATE include)

include(FetchContent)

# ---- libcurl ----
find_package(CURL REQUIRED)
target_include_directories(RealTimeDataPipeline PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(RealTimeDataPipeline PRIVATE ${CURL_LIBRARIES})