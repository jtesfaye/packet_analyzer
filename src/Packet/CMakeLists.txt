add_subdirectory(parsing)
add_subdirectory(layer2_protocols)
add_subdirectory(layer3_protocols)
add_subdirectory(layer4_protocols)

add_library(Packet
        PcapArray.cpp
        PcapFile.cpp
        PcapReader.cpp
)

target_include_directories(Packet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_include_directories(Packet PRIVATE ${Boost_INCLUDE_DIRS})