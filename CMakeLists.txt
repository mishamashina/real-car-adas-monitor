cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(real-car-adas-monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PARSER_LIB_NAME parser)

find_package(onnxruntime REQUIRED)

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
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)

FetchContent_MakeAvailable(googletest tomlplusplus)

target_link_libraries(RealCarMonitor ${PARSER_LIB_NAME} 
                                     onnxruntime::onnxruntime
                                     tomlplusplus::tomlplusplus
)
target_link_libraries(RunTests ${PARSER_LIB_NAME} 
                               gtest_main
)