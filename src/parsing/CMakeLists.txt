add_library(parsing
        InitialParser.cpp
        ${CMAKE_SOURCE_DIR}/include/parsing/ParseDispatcher.h
        DetailParser.cpp
        ${CMAKE_SOURCE_DIR}/include/parsing/ThreadPool.h
        ${CMAKE_SOURCE_DIR}/include/parsing/ThreadPool.tpp
)

target_include_directories(parsing PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(parsing PUBLIC layerx)
