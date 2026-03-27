cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(real-car-adas-monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PARSER_LIB_NAME parser)

find_package(OpenCV REQUIRED)
find_package(ONNX REQUIRED)

add_library(${PARSER_LIB_NAME} ${CMAKE_SOURCE_DIR}/src/obd_parser.cc)

add_executable(RealCarMonitor ${CMAKE_SOURCE_DIR}/src/dashboard.cc
                              ${CMAKE_SOURCE_DIR}/src/dms_hud.cc
                              ${CMAKE_SOURCE_DIR}/src/dms_monitor.cc
                              ${CMAKE_SOURCE_DIR}/src/main.cc
                              ${CMAKE_SOURCE_DIR}/src/onnx_classifier.cc
)
add_executable(RunTests ${CMAKE_SOURCE_DIR}/tests/test_obd_parser.cc)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

target_link_libraries(RealCarMonitor ${PARSER_LIB_NAME})
target_link_libraries(RunTests gtest_main)
target_link_libraries(RunTests ${PARSER_LIB_NAME})