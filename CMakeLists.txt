cmake_minimum_required(VERSION 3.29)
project(Sniffer)

set(CMAKE_CXX_STANDARD 23)

set(Qt6_DIR "/opt/homebrew/opt/qt")


set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Boost)
find_package(GTest REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Concurrent)
include_directories("/opt/homebrew/opt/libpcap/include")

add_subdirectory(src)
add_executable(main_src src/main.cpp)


target_link_libraries(main_src
        PRIVATE
        sniffer_main
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Concurrent
        capture
        controller
        layerx
        model
        Packet
        parsing
        util
        view
)

target_include_directories(main_src PRIVATE ${Boost_INCLUDE_DIRS} ${concurrentqueue_SOURCE_DIR})
add_subdirectory(tests)
target_include_directories(main_src PUBLIC ${CMAKE_SOURCE_DIR}/include)
