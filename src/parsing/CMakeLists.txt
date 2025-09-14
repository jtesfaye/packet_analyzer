add_library(parsing
        PacketParse.cpp
        ${CMAKE_SOURCE_DIR}/include/parsing/ParseDispatcher.h
)

target_include_directories(parsing PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(parsing PUBLIC layerx)
