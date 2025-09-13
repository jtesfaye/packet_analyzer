cmake_minimum_required(VERSION 3.29)
project(Sniffer)

set(CMAKE_CXX_STANDARD 23)

set(Qt6_DIR "/opt/homebrew/opt/qt")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Boost)
find_package(GTest REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)
include_directories("/opt/homebrew/opt/libpcap/include")

add_subdirectory(src)

add_executable(main_src src/main.cpp)
#add_executable(pcapFile_test tests/test_ThreadPool.cpp)
add_executable(parse_test tests/test_protcol.cpp)

target_link_libraries(parse_test PRIVATE
        Packet
        layer3_protocols
        GTest::gtest_main
        GTest::gmock
        pcap)

target_link_libraries(main_src
        PRIVATE
        sniffer_main
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        capture
        controller
        layerx
        model
        Packet
        parsing
        util
        view
)
target_include_directories(main_src PRIVATE ${Boost_INCLUDE_DIRS})

#target_link_libraries(test PRIVATE pcap GTest::GTest GTest::Main Qt6::Widgets Qt6::Core Qt6::Gui)