cmake_minimum_required(VERSION 3.29)
project(Sniffer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)

# Dependencies
find_package(Boost REQUIRED COMPONENTS
    interprocess
    unordered
    lockfree
    container
    container_hash
    json
    asio
)

find_package(GTest REQUIRED)
find_package(benchmark CONFIG REQUIRED)

# Let vcpkg handle Python + pybind
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

# Subdirectories
add_subdirectory(src)


# Main executable
add_executable(sniffer_core src/main.cpp)
target_link_libraries(sniffer_core PRIVATE
    sniffer_main
    benchmark::benchmark
    capture
    layerx
    Packet
    parsing
    util
    session
    stream
)

target_include_directories(sniffer_core PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Python module
pybind11_add_module(sniffer_py bindings/bindings.cpp)
target_link_libraries(sniffer_py PRIVATE
    sniffer_main
    capture
    layerx
    Packet
    parsing
    util
    session
    stream
)

target_compile_options(sniffer_py PRIVATE -fno-sanitize=address)
target_link_options(sniffer_py PRIVATE -fno-sanitize=address)
