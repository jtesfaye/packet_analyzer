add_library(layer2_protocols
        Ethernet.cpp
)

target_include_directories(layer2_protocols PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(layer2_protocols PUBLIC Packet util)
