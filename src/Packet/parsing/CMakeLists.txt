add_library(parsing
        PacketParse.cpp
        ../../util/PacketRead.cpp
        ${CMAKE_SOURCE_DIR}/include/parsing/ParseDispatcher.h
)

target_include_directories(parsing PUBLIC ${CMAKE_SOURCE_DIR}/include)
