cmake_minimum_required(VERSION 3.24)
project(tests)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(JSON_BuildTests OFF CACHE INTERNAL "")


include_directories(../include ../src)

file(GLOB sources ../src/*.cpp)
file(GLOB headers ../include/*.h)

add_executable(${PROJECT_NAME} InvertedIndex_test.cpp SearchServer_test.cpp ${sources} ${headers})

target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main PRIVATE nlohmann_json::nlohmann_json)