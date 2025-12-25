cmake_minimum_required(VERSION 3.29)
project(Sniffer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Boost REQUIRED COMPONENTS interprocess unordered lockfree container container_hash json)
find_package(GTest REQUIRED)
find_package(benchmark CONFIG REQUIRED)
include_directories("/opt/homebrew/opt/libpcap/include")

add_subdirectory(src)
add_executable(main_src src/main.cpp)
target_link_libraries(main_src
        PRIVATE
        sniffer_main
        benchmark::benchmark
        capture
        layerx
        Packet
        parsing
        util
)
target_include_directories(main_src PRIVATE ${concurrentqueue_SOURCE_DIR})
add_subdirectory(tests)
add_subdirectory(benchmarks)
target_include_directories(main_src PUBLIC ${CMAKE_SOURCE_DIR}/include)
