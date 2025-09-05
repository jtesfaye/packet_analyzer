add_subdirectory(capture)
add_subdirectory(controller)
add_subdirectory(model)
add_subdirectory(Packet)
add_subdirectory(util)
add_subdirectory(view)

add_library(sniffer_main)

target_include_directories(sniffer_main PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(sniffer_main PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui controller)