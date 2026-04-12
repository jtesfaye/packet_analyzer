add_subdirectory(capture)
add_subdirectory(layerx)
add_subdirectory(Packet)
add_subdirectory(parsing)
add_subdirectory(util)
add_subdirectory(session)
add_subdirectory(stream)

add_library(sniffer_main STATIC sniffer_api.cpp)

target_include_directories(sniffer_main PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(sniffer_main PUBLIC layerx parsing Packet capture util session stream)