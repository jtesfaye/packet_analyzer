add_subdirectory(capture)
add_subdirectory(layerx)
add_subdirectory(Packet)
add_subdirectory(parsing)
add_subdirectory(util)
add_subdirectory(session)
add_subdirectory(stream)

add_library(sniffer_main INTERFACE)

target_include_directories(sniffer_main INTERFACE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(sniffer_main INTERFACE layerx parsing Packet capture util session stream)