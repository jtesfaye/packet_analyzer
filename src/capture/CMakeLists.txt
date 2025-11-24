add_library(capture
        Offline.cpp
        Online.cpp
        PacketCapture.cpp
        ../session/CaptureSession.cpp
)

target_include_directories(capture PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(capture pcap Qt6::Core Qt6::Widgets benchmark::benchmark)