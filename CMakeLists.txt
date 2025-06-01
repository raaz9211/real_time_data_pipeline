cmake_minimum_required(VERSION 3.10)
project(RealTimeDataPipeline)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(RealTimeDataPipeline
    src/main.cpp
    src/FileDataSource.cpp
    src/DataProcessor.cpp
    src/Logger.cpp
    src/OutputWriter.cpp

)