
add_library(Packet
        PcapArray.cpp
        PcapFile.cpp
        PcapReader.cpp
)
target_include_directories(Packet PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(Packet PUBLIC pcap Boost::interprocess)