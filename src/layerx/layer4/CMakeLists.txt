add_library(layer4_protocols
        TCP.cpp
        ICMP.cpp
)

target_include_directories(layer4_protocols PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(layer4_protocols PUBLIC Packet)