
add_library(Packet
        PcapArray.cpp
        PcapFile.cpp
        PcapReader.cpp
)

target_include_directories(Packet PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(Packet PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(Packet PRIVATE pcap)