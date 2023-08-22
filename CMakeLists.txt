cmake_minimum_required(VERSION 3.24)
project(HellSearchEngine)

set(CMAKE_CXX_STANDARD 17)

set(JSON_BuildTests OFF CACHE INTERNAL "")

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

add_subdirectory(nlohmann_json)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} main.cpp ${sources} ${headers} include/ThreadPool.h)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
