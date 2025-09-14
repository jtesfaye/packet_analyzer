add_library(layer3_protocols
        IPv4.cpp

)

target_include_directories(layer3_protocols PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(layer3_protocols PUBLIC Packet)