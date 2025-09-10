add_library(layer2_protocols
        Ethernet.cpp
        ${CMAKE_SOURCE_DIR}/include/layer2_protocols/Layer2.h
)

target_include_directories(layer2_protocols PUBLIC ${CMAKE_SOURCE_DIR}/include)
