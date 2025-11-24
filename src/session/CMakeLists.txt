add_library(session
        CaptureSession.cpp
        StreamTable.cpp
        Stream.cpp
)

target_include_directories(session PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(session PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(session PUBLIC capture parsing util)