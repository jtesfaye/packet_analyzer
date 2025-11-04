add_library(parsing
        InitialParser.cpp
        ${CMAKE_SOURCE_DIR}/include/parsing/ParseDispatcher.h
        DetailParser.cpp
        ParsingEngine.cpp
)

target_include_directories(parsing PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(parsing PUBLIC layerx)
