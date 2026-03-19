cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(real-car-adas-monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(ONNX REQUIRED)

file(GLOB REAL_CAR_MONITOR_SRC ${CMAKE_SOURCE_DIR}/src/*.cc)
file(GLOB RUN_TESTS_SRC ${CMAKE_SOURCE_DIR}/tests/*.cc)

add_executable(RealCarMonitor ${REAL_CAR_MONITOR_SRC})
add_executable(RunTests ${RUN_TESTS_SRC})

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

target_link_libraries(RunTests gtest_main)