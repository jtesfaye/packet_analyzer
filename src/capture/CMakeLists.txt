add_library(capture
        OfflineCapture.cpp
        OnlineCapture.cpp
        PacketCapture.cpp
)
target_include_directories(capture PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(capture PUBLIC pcap Boost::interprocess Boost::lockfree)