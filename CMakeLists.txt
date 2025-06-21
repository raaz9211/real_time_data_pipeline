cmake_minimum_required(VERSION 3.15)
project(RealTimeDataPipeline)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable warnings and debug symbols
add_compile_options(-Wall -Wextra -pedantic -O2 -g)

# -----------------------------------------
# Find Dependencies
# -----------------------------------------
find_package(CURL REQUIRED)

# -----------------------------------------
# Include Directories
# -----------------------------------------
include_directories(include third_party)

# -----------------------------------------
# Core Library (src/*.cpp)
# -----------------------------------------
file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS
    src/*.cpp
)
list(REMOVE_ITEM CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(core_lib ${CORE_SRC})
target_include_directories(core_lib PUBLIC include third_party)
target_link_libraries(core_lib PUBLIC ${CURL_LIBRARIES} pthread sqlite3)

# -----------------------------------------
# Main Executable (src/main.cpp)
# -----------------------------------------
add_executable(main_pipeline src/main.cpp)
target_link_libraries(main_pipeline PRIVATE core_lib)

# -----------------------------------------
# GoogleTest - Test Runner
# -----------------------------------------
enable_testing()

file(GLOB_RECURSE TEST_SRC CONFIGURE_DEPENDS
    tests/*.cpp
)

add_executable(test_runner ${TEST_SRC})
target_link_libraries(test_runner PRIVATE core_lib gtest gtest_main pthread)
target_include_directories(test_runner PRIVATE include third_party)

add_test(NAME AllTests COMMAND test_runner)

# -----------------------------------------
# Optional Future Tools (disabled for now)
# -----------------------------------------
# add_executable(metrics_server tools/metrics_server.cpp)
# target_link_libraries(metrics_server PRIVATE core_lib pthread)
